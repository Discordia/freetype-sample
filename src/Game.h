#pragma once

#include <memory>
#include <core/Dimension.h>

using std::shared_ptr;
using std::unique_ptr;

class FTFont;
class FontAtlas;
class StreamFactory;

class Game
{
public:

    //!
    //!
    //!
    Game(shared_ptr<StreamFactory> streamFactory);

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
    shared_ptr<FontAtlas> fontAtlas;

    //!
    shared_ptr<FTFont> font;

    //!
    shared_ptr<StreamFactory> streamFactory;
};