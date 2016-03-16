#pragma once

#include <memory>
#include <core/App.h>
#include <core/Dimension.h>
#include <font/FontGeometry.h>

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

class FTFont;
class FontAtlas;
class StreamFactory;
class FontBatchRenderer;

class FontPrototype : public App
{
public:

    //!
    //!
    //!
    FontPrototype(shared_ptr<StreamFactory> streamFactory);

    //!
    //!
    //!
    ~FontPrototype();

    //!
    //!
    //!
    void init(const Dimension& windowSize);

    //!
    //!
    //!
    void render();

private:

    //!
    shared_ptr<FontBatchRenderer> renderer;

    //!
    shared_ptr<FontAtlas> fontAtlas;

    //!
    weak_ptr<FTFont> font;
    shared_ptr<FontGeometry> neonText;
    shared_ptr<FontGeometry> foxText;

    //!
    shared_ptr<StreamFactory> streamFactory;
};