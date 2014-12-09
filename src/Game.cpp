#include <Game.h>
#include <ShaderProgram.h>
#include <Log.h>
#include <FontAtlas.h>
#include <lodepng.h>

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
        "varying vec2 textureCoord;\n"
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

    GLint projectionLoc = glGetUniformLocation(shader->getProgramId(), "Projection");
    glUniformMatrix4fv(projectionLoc, 1, 0, identity);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    /*
    fontAtlas = shared_ptr<FontAtlas>(new FontAtlas(true));
    fontAtlas->addFont("LiberationMono-Regular.ttf", 16, " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz");
    fontAtlas->create();*/

    /*glBindTexture(GL_TEXTURE_2D, fontAtlas->getTextureId());
    GLint textureSamplerLoc = glGetUniformLocation(shader->getProgramId(), "tex");
    glUniform1i(textureSamplerLoc, 0); */

    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, "assets/test.png");

    //if there's an error, display it
    if (error) LOGE("decoder error");

    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    size_t u2 = 1;
    while (u2 < width) u2 *= 2;
    size_t v2 = 1;
    while (v2 < height) v2 *= 2;
    // Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
    u3 = (float) width / u2;
    v3 = (float) height / v2;

    // Make power of two version of the image.
    std::vector<unsigned char> image2(u2 * v2 * 4);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            for (size_t c = 0; c < 4; c++)
            {
                image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
            }
        }
    }

    // Enable the texture for OpenGL.
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);

    GLint textureSamplerLoc = glGetUniformLocation(shader->getProgramId(), "tex");
    glUniform1i(textureSamplerLoc, 0);
}

void Game::render()
{

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat vVertices[] = {
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);


    const GLfloat textureCoords[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    // Load the texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, textureCoords);
    glEnableVertexAttribArray(1);

    GLubyte  indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
}
