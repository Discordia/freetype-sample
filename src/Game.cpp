#include <Game.h>
#include <ShaderProgram.h>
#include <Log.h>
#include <FontAtlas.h>
#include <lodepng.h>
#include <BufferObject.h>

#include <string>
#include <iostream>


using std::string;

#define LOG_TAG "Game"

const string vShaderSource =
        "attribute vec4 position;      \n"
        "uniform mat4 Projection;\n"
        "uniform mat4 ModelView;\n"
        "\n"
        "void main()                    \n"
        "{                              \n"
        "   gl_Position = Projection * ModelView * position;    \n"
        "}                              \n";

const string fShaderSource =
        "precision mediump float;                       \n"
        "void main()                                    \n"
        "{                                              \n"
        "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);    \n"
        "}                                              \n";


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
            "    gl_FragColor = vec4(diffuseColor.rgb, texture2D(tex, textureCoord).r * diffuseColor.a);\n"
        "}";


Game::Game()
{
}

void Game::init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //
    // Vertex Buffer
    // x, y, z, u, v
    //

    GLfloat vertices[] = {
            0.0f,  320.0f, 0.3f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.3f, 0.0f, 1.0f,
            480.0f, 0.0f, 0.3f, 1.0f, 1.0f,
            480.0f, 320.0f, 0.3f, 1.0f, 0.0f
    };

    vertexBuffer = BufferObject::createVertexBuffer(20 * sizeof(GLfloat));
    vertexBuffer->fill(0, 5 * 4 * 4, vertices);

    //
    // Index buffer
    //

    GLubyte  indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    indexBuffer = BufferObject::createIndexBuffer(6 * sizeof(GLubyte));
    indexBuffer->fill(0, 6 * sizeof(GLubyte), indices);

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

    GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLint diffuseColorLoc = glGetUniformLocation(shader->getProgramId(), "diffuseColor");
    glUniform4fv(diffuseColorLoc, 1, color);

    //
    // Texture
    //

    fontAtlas = shared_ptr<FontAtlas>(new FontAtlas(true));
    fontAtlas->addFont("LiberationMono-Regular.ttf", 128, " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz");
    fontAtlas->create();

    glBindTexture(GL_TEXTURE_2D, fontAtlas->getTextureId());
    GLint textureSamplerLoc = glGetUniformLocation(shader->getProgramId(), "tex");
    glUniform1i(textureSamplerLoc, 0);
}

void Game::render()
{
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // Setup the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * 4, 0);
    glEnableVertexAttribArray(0);

    // Setup the texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * 4, (const void*)(3 * 4));
    glEnableVertexAttribArray(1);

    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}
