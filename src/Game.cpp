#include <Game.h>
#include <core/ShaderProgram.h>
#include <font/FontAtlas.h>
#include <core/BufferObject.h>
#include <font/FontBatchRenderer.h>

#include <iostream>

#define LOG_TAG "Game"

Game::Game(shared_ptr<StreamFactory> streamFactory)
    : streamFactory(streamFactory)
{
}

void Game::init()
{
    glClearColor(1.0f, 0.20f, 0.60f, 1.0f);

    FontBatchRenderer::getRenderer().init();

    fontAtlas = shared_ptr<FontAtlas>(new FontAtlas(streamFactory, 1024, 1024));
    font = fontAtlas->addFont("LiberationMono-Regular.ttf", 16, " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz");
    fontAtlas->create();
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    font->drawString(10, 10, "NEON GENESIS EVANGELION", 0x000000, 1.0f);

    font->drawString(10, 100, "The quick brown fox jumped over the lazy dog.", 0x000000, 1.0f);

    FontBatchRenderer::getRenderer().render();
}
