#include <Game.h>
#include <ShaderProgram.h>
#include <Log.h>
#include <FontAtlas.h>

#include <string>

using std::string;

#define LOG_TAG "Game"

const string vShaderSource =
        "attribute vec4 vPosition;      \n"
        "void main()                    \n"
        "{                              \n"
        "   gl_Position = vPosition;    \n"
        "}                              \n";

const string fShaderSource =
        "precision mediump float;                       \n"
        "void main()                                    \n"
        "{                                              \n"
        "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);    \n"
        "}                                              \n";

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
    glBindAttribLocation(shader->getProgramId(), 0, "vPosition");

    shader->link();

    if (!shader->isLinked()) LOGE("Shader failed to link");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    FontAtlas fontAtlas(true);
    fontAtlas.addFont("LiberationMono-Regular.ttf", 16, " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz");
    fontAtlas.create();
}

void Game::render()
{
    GLfloat vVertices[] =
    {
        0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    shader->use();

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
