#include <Game.h>
#include <ShaderProgram.h>
#include <Log.h>
#include <FontAtlas.h>

#include <string>

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
        "varying mediump vec2 textureCoord;\n"
        "\n"
        "uniform sampler2D tex;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(tex, textureCoord);\n"
        "}";


Game::Game()
{
}

void Game::init()
{
    shader = ShaderProgram::create();

    shared_ptr<ShaderObject> vertexShader = ShaderObject::create(GL_VERTEX_SHADER, vShaderSource);
    if (!vertexShader->isCompiled()) LOGE("Vertext shader failed to compile.");
    shader->attachShader(vertexShader);

    shared_ptr<ShaderObject> fragmentShader = ShaderObject::create(GL_FRAGMENT_SHADER, fShaderSource);
    if (!fragmentShader->isCompiled()) LOGE("Fragment shader failed to compile.");
    shader->attachShader(fragmentShader);

    // Bind vPosition to attribute 0
    glBindAttribLocation(shader->getProgramId(), 0, "position");

    shader->link();

    if (!shader->isLinked()) LOGE("Shader failed to link");

    shader->use();

    const GLfloat identity[] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1};
    GLint modelViewLoc = glGetUniformLocation(shader->getProgramId(), "ModelView");
    glUniformMatrix4fv(modelViewLoc, 1, 0, identity);

    GLint projectionLoc = glGetUniformLocation(shader->getProgramId(), "Projection");
    glUniformMatrix4fv(projectionLoc, 1, 0, identity);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    fontAtlas = shared_ptr<FontAtlas>(new FontAtlas(true));
    fontAtlas->addFont("LiberationMono-Regular.ttf", 16, " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz");
    fontAtlas->create();
}

void Game::render()
{
    GLfloat vVertices[] =
    {
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.5f
    };

    const GLfloat textureCoords[] = {
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
    };

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    GLubyte  indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
}
