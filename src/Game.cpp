#include <Game.h>
#include <ShaderProgram.h>
#include <FontAtlas.h>
#include <BufferObject.h>
#include <FontBatchRenderer.h>

#include <iostream>

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

    font->drawString(100, 100, "A", 0x000000, 1.0f);
    /*font->drawString(150, 150, "Testing..2", 0xFF0000, 1.0f);
    font->drawString(150, 150, "Testing..3", 0x00FF00, 1.0f); */

    FontBatchRenderer::getRenderer().render();
}
