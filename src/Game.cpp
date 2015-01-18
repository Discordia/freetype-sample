#include <Game.h>
#include <ShaderProgram.h>
#include <Log.h>
#include <FontAtlas.h>
#include <lodepng.h>
#include <BufferObject.h>

#include <string>
#include <iostream>
#include "FontBatchRenderer.h"


using std::string;

#define LOG_TAG "Game"

Game::Game()
{
}

void Game::init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    FontBatchRenderer::getRenderer().init();

    fontAtlas = shared_ptr<FontAtlas>(new FontAtlas(true));
    font = fontAtlas->addFont("LiberationMono-Regular.ttf", 128, " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz");
    fontAtlas->create();
}

void Game::render()
{
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    font->drawString(100, 100, "Testing...", 0x000000, 1.0f);
}
