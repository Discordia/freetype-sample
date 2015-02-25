#include "FontBatchRenderer.h"
#include "BufferObject.h"
#include "ShaderProgram.h"
#include "Log.h"
#include <FTFontChar.h>

#include <cassert>

#define LOG_TAG "FontBatchRenderer"

const int VERTEX_STRIDE = 5;

const string texVertexShader =
        "attribute vec4 position;\n"
        "attribute vec2 texture_coord;\n"
        "\n"
        "varying vec2 textureCoord;\n"
        "\n"
        "uniform mat4 Projection;\n"
        "uniform mat4 ModelView;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = Projection * ModelView * position;\n"
        "    textureCoord =  texture_coord;\n"
        "}";

const string texFragShader =
        "precision mediump float;\n"
        "varying vec2 textureCoord;\n"
        "uniform sampler2D tex;\n"
        "uniform vec4 diffuseColor;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = vec4(diffuseColor.rgb, texture2D(tex, textureCoord).a * diffuseColor.a);\n"
        "}";


FontBatchRenderer FontBatchRenderer::renderer;

FontBatchRenderer::FontBatchRenderer()
    : numQuads(0), textureId(0), cacheSize(150), drawCallCount(0)

{
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

void FontBatchRenderer::addQuad(const float* vertices, const float* texCoords)
{
    uint32 currIndex = (uint32) (numQuads * VERTICES_PER_QUAD * VERTEX_STRIDE * sizeof(float));

    for (int n = 0; n < VERTICES_PER_QUAD; n++)
    {
        // x,y,z
        vertexBuffer->fill((uint32 const)(currIndex + (n * VERTEX_STRIDE * sizeof(float))), 3 * sizeof(float), &vertices[n * 3]);
        // u,v
        vertexBuffer->fill((uint32 const) (currIndex + (n * VERTEX_STRIDE * sizeof(float)) + 3 * sizeof(float)) , 2 * sizeof(float), &texCoords[n * 2]);
    }

    numQuads++;
}

void FontBatchRenderer::init()
{
    vertexBuffer = BufferObject::createVertexBuffer((uint32 const) (cacheSize * VERTICES_PER_QUAD * VERTEX_STRIDE * sizeof(GLfloat)));
    indexBuffer = BufferObject::createIndexBuffer((uint32 const) (cacheSize * INDICES_PER_QUAD  * sizeof(GLubyte)));

    // Indices
    GLubyte* indices = new GLubyte[(uint32) (cacheSize * INDICES_PER_QUAD)];
    for (int n = 0; n < cacheSize; n++)
    {
        indices[n * INDICES_PER_QUAD]     = (GLubyte) (n * VERTICES_PER_QUAD);
        indices[n * INDICES_PER_QUAD + 1] = (GLubyte) (n * VERTICES_PER_QUAD + 1);
        indices[n * INDICES_PER_QUAD + 2] = (GLubyte) (n * VERTICES_PER_QUAD + 2);
        indices[n * INDICES_PER_QUAD + 3] = (GLubyte) (n * VERTICES_PER_QUAD + 2);
        indices[n * INDICES_PER_QUAD + 4] = (GLubyte) (n * VERTICES_PER_QUAD + 3);
        indices[n * INDICES_PER_QUAD + 5] = (GLubyte) (n * VERTICES_PER_QUAD);
    }

    indexBuffer->fill(0, (uint32) (cacheSize * INDICES_PER_QUAD * sizeof(GLubyte)), indices);
    delete[] indices;

    //
    // Shader
    //

    shader = ShaderProgram::create();

    shared_ptr<ShaderObject> vertexShader = ShaderObject::create(GL_VERTEX_SHADER, texVertexShader);
    if (!vertexShader->isCompiled()) LOGE("Vertext shader failed to compile.");
    shader->attachShader(vertexShader);

    shared_ptr<ShaderObject> fragmentShader = ShaderObject::create(GL_FRAGMENT_SHADER, texFragShader);
    if (!fragmentShader->isCompiled()) LOGE("Fragment shader failed to compile.");
    shader->attachShader(fragmentShader);

    // Bind vPosition to attribute 0
    glBindAttribLocation(shader->getProgramId(), 0, "position");
    glBindAttribLocation(shader->getProgramId(), 1, "texture_coord");

    shader->link();

    if (!shader->isLinked()) LOGE("Shader failed to link");

    shader->use();

    const GLfloat identity[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
    GLint modelViewLoc = glGetUniformLocation(shader->getProgramId(), "ModelView");
    glUniformMatrix4fv(modelViewLoc, 1, 0, identity);

    GLfloat ortho[16];
    oglOrthof(ortho, 0.0f, 480.0f, 0.0f, 320.0f, -1.0f, 1.0f);
    GLint projectionLoc = glGetUniformLocation(shader->getProgramId(), "Projection");
    glUniformMatrix4fv(projectionLoc, 1, 0, ortho);
}

void FontBatchRenderer::render()
{
    if (numQuads == 0) return;
    assert(numQuads * INDICES_PER_QUAD < 0xffff);

    // TODO: use culling?
    // glFrontFace(GL_CW);
    // glEnable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    shader->use();

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set texture
    glBindTexture(GL_TEXTURE_2D, textureId);
    GLint textureSamplerLoc = glGetUniformLocation(shader->getProgramId(), "tex");
    glUniform1i(textureSamplerLoc, 0);

    // Set color
    const float inv256 = 1.0f / 256.0f;

    float red = ((color>>16)&0xff) * inv256;
    float green = ((color>>8)&0xff) * inv256;
    float blue = (color&0xff) * inv256;

    GLfloat color[] = {red, green, blue, alpha};
    GLint diffuseColorLoc = glGetUniformLocation(shader->getProgramId(), "diffuseColor");
    glUniform4fv(diffuseColorLoc, 1, color);

    //
    // Draw
    //

    // Setup the vertex data
    // TODO: can we only have two floats per vertex?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_STRIDE, 0);
    glEnableVertexAttribArray(0);

    // Setup the texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_STRIDE, (const void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Draw
    glDrawElements(GL_TRIANGLES, (GLsizei)(numQuads * INDICES_PER_QUAD), GL_UNSIGNED_BYTE, 0);

    // Update statistics
    drawCallCount++;
    quadCount += numQuads;
    numQuads = 0;
}

void FontBatchRenderer::release()
{
    cacheSize = 0;
}
