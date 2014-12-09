#pragma once

#include <memory>

using std::shared_ptr;
using std::unique_ptr;

class ShaderProgram;
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
    shared_ptr<ShaderProgram> shader;

    shared_ptr<FontAtlas> fontAtlas;
};