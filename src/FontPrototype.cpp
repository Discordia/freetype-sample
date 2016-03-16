#include <FontPrototype.h>
#include <core/BufferObject.h>
#include <core/ShaderProgram.h>
#include <font/FontAtlas.h>
#include <font/FontBatchRenderer.h>
#include <font/FTFont.h>

#define LOG_TAG "Game"

FontPrototype::FontPrototype(shared_ptr<StreamFactory> streamFactory)
    : streamFactory(streamFactory), renderer(new FontBatchRenderer())
{
    linkIssueFix();
}

FontPrototype::~FontPrototype()
{
}

void FontPrototype::init(const Dimension& windowSize)
{
    glClearColor(1.0f, 0.20f, 0.60f, 1.0f);

    renderer->init(windowSize);

    fontAtlas = shared_ptr<FontAtlas>(new FontAtlas(streamFactory, 1024, 1024));
    font = fontAtlas->addFont("LiberationMono-Regular.ttf", 16, " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyz");
    fontAtlas->create();

    shared_ptr<FTFont> fontPtr = font.lock();
    neonText = fontPtr->calcVertices(10, 10, "NEON GENESIS EVANGELION", 0x000000, 1.0f);
    foxText = fontPtr->calcVertices(10, 100, "The quick brown fox jumped over the lazy dog.", 0x000000, 1.0f);
}

void FontPrototype::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    renderer->render(neonText);
    renderer->render(foxText);
}

App* createApp(shared_ptr<StreamFactory> streamFactory) {
    return new FontPrototype(streamFactory);
}
