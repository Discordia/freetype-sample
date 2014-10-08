#pragma once

#include <memory>

using std::shared_ptr;

class ShaderProgram;

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
};