#pragma once

#include <memory>
#include <font/VertexTypes.h>

using std::shared_ptr;

class FontGeometry
{
public:
    FontGeometry(unsigned int textureId, int color, float alpha, int quads, TexturedVertex* vertices)
        : textureId(textureId), color(color), alpha(alpha), quads(quads), vertices(vertices)
    {
    }

    ~FontGeometry()
    {
        delete[] vertices;
    }

    const unsigned int& getTextureId()
    {
        return textureId;
    }

    const int& getColor()
    {
        return color;
    }

    const float& getAlpha()
    {
        return alpha;
    }

    const int& getNrQuads()
    {
        return quads;
    }

    TexturedVertex* getVertices()
    {
        return vertices;
    }


private:
    unsigned int textureId;
    int color;
    float alpha;
    int quads;
    TexturedVertex* vertices;
};
