#pragma once

#include <memory>

using std::shared_ptr;
using std::unique_ptr;

class FTFont;
class FontAtlas;

class Game
{
public:

    //!
    //!
    //!
    Game();

    //!
    //!
    //!
    void init();

    //!
    //!
    //!
    void render();

private:

    //
    shared_ptr<FontAtlas> fontAtlas;

    //
    shared_ptr<FTFont> font;
};