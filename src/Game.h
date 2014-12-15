#pragma once

#include <memory>

using std::shared_ptr;
using std::unique_ptr;

class BufferObject;
class FontAtlas;
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
    shared_ptr<BufferObject> vertexBuffer;

    //
    shared_ptr<BufferObject> indexBuffer;

    //
    shared_ptr<ShaderProgram> shader;

    //
    shared_ptr<FontAtlas> fontAtlas;
};