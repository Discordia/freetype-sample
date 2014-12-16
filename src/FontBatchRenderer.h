#pragma once

#include <OpenGL.h>

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
    void setAttributes(unsigned int textureId, int color, float alpha, bool transparent);

    //!
    //!
    //!
    void addQuad(const float* texCoords, const float* vertices);

    //!
    //!
    //!
    void render();

    //!
    //!
    //!
    void reallocate(int sizeIncrease);

private:

    //!
    //!
    //!
    void release();

private:

    //! Instance
    static FontBatchRenderer renderer;

    //! Render data
    float* vertexData;		// storage of vertex array
    GLushort* indices;	    // indices of quads
    int numQuads;			// current number of quads waiting to be rendered
    int cacheSize;		    // current amount of quads that can fit in the arrays

    //! Render properties
    float alpha;			// alpha value of current quads
    int color;			    // colour tint of current quads
    GLuint textureId;		// texture of current quads
    bool transparent;		// are current quads transparent

    //! Statistics
    int drawCallCount;
    int quadCount;
};