#pragma once

#include <OpenGL.h>
#include <memory>

using std::shared_ptr;

class BufferObject;
class ShaderProgram;

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
    static FontBatchRenderer& getRenderer();

    //!
    //!
    //!
    void setAttributes(unsigned int textureId, int color, float alpha);

    //!
    //!
    //!
    void addQuad(const float* vertices, const float* texCoords);

    //!
    //!
    //!
    void init();

    //!
    //!
    //!
    void render();

private:

    //!
    //!
    //!
    void release();

private:

    //! Instance
    static FontBatchRenderer renderer;

    // Render state
    shared_ptr<BufferObject> vertexBuffer;
    shared_ptr<BufferObject> indexBuffer;
    shared_ptr<ShaderProgram> shader;

    int numQuads;			// current number of quads waiting to be rendered
    int cacheSize;		    // current amount of quads that can fit in the arrays

    //! Render properties
    float alpha;			// alpha value of current quads
    int color;			    // colour tint of current quads
    GLuint textureId;		// texture of current quads

    //! Statistics
    int drawCallCount;
    int quadCount;
};