#include <font/FontBatchRenderer.h>
#include <core/BufferObject.h>
#include <core/ShaderProgram.h>
#include <core/Log.h>
#include <font/FontGeometry.h>
#include <font/FTFontChar.h>

#include <cassert>

static const Logger LOGGER = Logger::create("FontBatchRenderer");

const int VERTEX_STRIDE = 4;

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
        "varying vec2 textureCoord;\n"
        "uniform sampler2D tex;\n"
        "uniform vec4 diffuseColor;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = vec4(diffuseColor.rgb, texture2D(tex, textureCoord).a * diffuseColor.a);\n"
        "}";


FontBatchRenderer::FontBatchRenderer()
    : cacheSize(150)
{
}

FontBatchRenderer::~FontBatchRenderer()
{
    cacheSize = 0;
}

void FontBatchRenderer::init(const Dimension& windowSize)
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
    if (!vertexShader->isCompiled()) LOGGER.logf(LOG_ERROR, "Vertext shader failed to compile.");
    shader->attachShader(vertexShader);

    shared_ptr<ShaderObject> fragmentShader = ShaderObject::create(GL_FRAGMENT_SHADER, texFragShader);
    if (!fragmentShader->isCompiled()) LOGGER.logf(LOG_ERROR, "Fragment shader failed to compile.");
    shader->attachShader(fragmentShader);

    shader->link();
    if (!shader->isLinked()) LOGGER.logf(LOG_ERROR, "Shader failed to link");

    // Bind vPosition to attribute 0
    glBindAttribLocation(shader->getProgramId(), 0, "position");
    glBindAttribLocation(shader->getProgramId(), 1, "texture_coord");


    shader->use();

    const GLfloat identity[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
    GLint modelViewLoc = glGetUniformLocation(shader->getProgramId(), "ModelView");
    glUniformMatrix4fv(modelViewLoc, 1, 0, identity);

    GLfloat ortho[16];
    oglOrthof(ortho, 0.0f, windowSize.width, 0.0f, windowSize.height, -1.0f, 1.0f);
    GLint projectionLoc = glGetUniformLocation(shader->getProgramId(), "Projection");
    glUniformMatrix4fv(projectionLoc, 1, 0, ortho);
}

void FontBatchRenderer::destroy()
{
    shader.reset();
    indexBuffer.reset();
    vertexBuffer.reset();
}

void FontBatchRenderer::render(shared_ptr<FontGeometry> fontGeometry)
{
    const unsigned int& textureId = fontGeometry->getTextureId();
    const int& color = fontGeometry->getColor();
    const float& alpha = fontGeometry->getAlpha();
    const int& quads = fontGeometry->getNrQuads();
    TexturedVertex* vertices = fontGeometry->getVertices();

    vertexBuffer->fill(0, quads * VERTICES_PER_QUAD * VERTEX_STRIDE * sizeof(float), &vertices[0]);
    if (quads == 0) return;
    assert(quads * INDICES_PER_QUAD < 0xffff);

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

    GLfloat colorV[] = {red, green, blue, alpha};
    GLint diffuseColorLoc = glGetUniformLocation(shader->getProgramId(), "diffuseColor");
    glUniform4fv(diffuseColorLoc, 1, colorV);

    //
    // Draw
    //

    // Setup the vertex data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_STRIDE, 0);
    glEnableVertexAttribArray(0);

    // Setup the texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_STRIDE, (const void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Draw
    glDrawElements(GL_TRIANGLES, (GLsizei)(quads * INDICES_PER_QUAD), GL_UNSIGNED_BYTE, 0);
}


