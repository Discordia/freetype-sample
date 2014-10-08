#include <Game.h>
#include <ShaderProgram.h>

#include <string>
#include "Log.h"

using std::string;

const string vShaderSource =
        "#version 100                             \n"
        "layout(location = 0) in vec4 vPosition;  \n"
        "void main()                              \n"
        "{                                        \n"
        "   gl_Position = vPosition;              \n"
        "}                                        \n";

const string fShaderSource =
        "#version 100                                 \n"
        "precision mediump float;                     \n"
        "out vec4 fragColor;                          \n"
        "void main()                                  \n"
        "{                                            \n"
        "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
        "}                                            \n";


Game::Game()
{
}

void Game::init()
{
    shader = ShaderProgram::create();

    shared_ptr<ShaderObject> vertexShader = ShaderObject::create(GL_VERTEX_SHADER, vShaderSource);

    if (!vertexShader->isCompiled()) LOGE("Game", "Vertext shader failed to compile.");

    shader->attachShader(vertexShader);

    shared_ptr<ShaderObject> fragmentShader = ShaderObject::create(GL_FRAGMENT_SHADER, fShaderSource);

    if (!fragmentShader->isCompiled()) LOGE("Game", "Fragment shader failed to compile.");

    shader->attachShader(fragmentShader);

    shader->link();

    if (!shader->isLinked()) LOGE("Game", "Shader failed to link");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
