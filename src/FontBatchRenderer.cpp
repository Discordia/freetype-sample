#include "FontBatchRenderer.h"
#include <FTFontChar.h>

#include <cassert>
#include <cstring>

const int VERTEX_STRIDE = 5;

FontBatchRenderer FontBatchRenderer::renderer;

FontBatchRenderer::FontBatchRenderer()
    : numQuads(0), vertexData(nullptr), indices(nullptr), textureId(0), cacheSize(0), drawCallCount(0)

{
    reallocate(150);
}

FontBatchRenderer::~FontBatchRenderer()
{
    release();
}

FontBatchRenderer& FontBatchRenderer::getRenderer()
{
    return renderer;
}

void FontBatchRenderer::setAttributes(unsigned int textureId, int color, float alpha)
{
    if (alpha > 1.0f) alpha = 1.0f;
    if (this->textureId != textureId ||
        this->color != color ||
        this->alpha != alpha)
    {
        render();

        this->textureId = textureId;
        this->color = color;
        this->alpha = alpha;
    }
}

void FontBatchRenderer::addQuad(const float* texCoords, const float* vertices)
{
    if (numQuads >= cacheSize)
    {
        reallocate(20);
    }

    int currIndex = numQuads * VERTICES_PER_QUAD * VERTEX_STRIDE;
    for (int n = 0; n < VERTICES_PER_QUAD; n++)
    {
        // x,y,z: note: do not need to copy 3rd coordinate, just zero it
        vertexData[currIndex+n * VERTEX_STRIDE]   = vertices[n * COMP_VERT_POS];
        vertexData[currIndex+n* VERTEX_STRIDE + 1] = vertices[n * COMP_VERT_POS + 1];
        // u,v
        vertexData[currIndex+n* VERTEX_STRIDE + 3] = texCoords[n * COMP_VERT_TEX];
        vertexData[currIndex+n* VERTEX_STRIDE + 4] = texCoords[n * COMP_VERT_TEX + 1];
    }

    numQuads++;
}

void FontBatchRenderer::render()
{
    if (numQuads == 0) return;

    glDisable(GL_LIGHTING);
    glFrontFace(GL_CW);

    glEnable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // set orthographic, so that we are drawing billboard style using screen dimensions
    glOrthof(0, (float) 720, (float) 480, 0, -50.0f, 50.0f);
    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glClientActiveTexture(GL_TEXTURE1);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    if (transparent || alpha < 0.99f)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    glBindTexture(GL_TEXTURE_2D, textureId);


    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    if (alpha > 0.99f && color == WHITE)
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }
    else
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        const float inv256 = 1.0f / 256.0f;

        float red = ((color>>16)&0xff) * inv256;
        float green = ((color>>8)&0xff) * inv256;
        float blue = (color&0xff) * inv256;

        if (alpha > 1.0f)
        {
            alpha = 1.0f;
        }

        glColor4f(red, green, blue, alpha);
    }

    /*
     Set the vertex pointer.
     param 1: Number of coordinates per vertex; must be 2, 3, or 4.
     param 2: GL_FIXED for CommonLite and GL_FLOAT for Common profile.
     param 3: Specifies the byte offset between consecutive vertexes.
     param 4: Offset to the start of the first vertex into the VBO.
     */
    glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_STRIDE, vertexData);
    glTexCoordPointer(COMP_VERT_TEX, GL_FLOAT, sizeof(float) * VERTEX_STRIDE, &vertexData[3]);
    assert(numQuads * INDICES_PER_QUAD < 0xffff);
    glDrawElements(GL_TRIANGLES, (GLsizei)(numQuads * INDICES_PER_QUAD), GL_UNSIGNED_SHORT, indices);

    // Update statistics
    drawCallCount++;
    quadCount += numQuads;
    numQuads = 0;

    // revert back to previous state
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void FontBatchRenderer::reallocate(int sizeIncrease)
{
    int newSize = numQuads + sizeIncrease;
    float* pVertices = new float[newSize * VERTICES_PER_QUAD * VERTEX_STRIDE];	// x,y,z
    memset(pVertices, 0, newSize * VERTICES_PER_QUAD * VERTEX_STRIDE * sizeof(float));
    GLushort* pIndices = new GLushort[newSize * INDICES_PER_QUAD];   // 2 triangles per quad
    // indices never change
    for (int n = 0; n < newSize; n++)
    {
        pIndices[n * INDICES_PER_QUAD]     = (GLshort) n * VERTICES_PER_QUAD;
        pIndices[n * INDICES_PER_QUAD + 1] = (GLshort) n * VERTICES_PER_QUAD + 1;
        pIndices[n * INDICES_PER_QUAD + 2] = (GLshort) n * VERTICES_PER_QUAD + 2;
        pIndices[n * INDICES_PER_QUAD + 3] = (GLshort) n * VERTICES_PER_QUAD + 1;
        pIndices[n * INDICES_PER_QUAD + 4] = (GLshort) n * VERTICES_PER_QUAD + 3;
        pIndices[n * INDICES_PER_QUAD + 5] = (GLshort) n * VERTICES_PER_QUAD + 2;
    }

    if (numQuads > 0)
    {
        int numVert = numQuads * VERTICES_PER_QUAD;
        memcpy(pVertices, vertexData, numVert * VERTEX_STRIDE * sizeof(float));
        release();
    }
    vertexData = pVertices;
    indices = pIndices;
    cacheSize = newSize;
}

void FontBatchRenderer::release()
{
    if (vertexData != nullptr)
    {
        delete[] vertexData;
        vertexData = nullptr;
    }
    if (indices != nullptr)
    {
        delete[] indices;
        indices = nullptr;
    }

    cacheSize = 0;
}
