#pragma once

#include <memory>
#include <core/OpenGL.h>
#include <core/Dimension.h>

using std::shared_ptr;

class BufferObject;
class ShaderProgram;
class FontGeometry;

class FontBatchRenderer
{
public:

    //!
    //!
    //!
    FontBatchRenderer();

    //!
    //!
    //!
    ~FontBatchRenderer();

    //!
    //!
    //!
    void init(const Dimension& windowSize);

    //!
    //!
    //!
    void destroy();

    //!
    //!
    //!
    void render(shared_ptr<FontGeometry> fontGeometry);

private:

    //! render state
    shared_ptr<BufferObject> vertexBuffer;
    shared_ptr<BufferObject> indexBuffer;
    shared_ptr<ShaderProgram> shader;

    //! current amount of quads that can fit in the arrays
    int cacheSize;
};