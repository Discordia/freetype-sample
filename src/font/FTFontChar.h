#pragma once

#include <font/VertexTypes.h>

struct FT_GlyphRec_;

const int VERTICES_PER_QUAD = 4;
const int INDICES_PER_QUAD = 6;
const int COMP_VERT_POS = 2;

class FTFontChar
{
public:

    //!
    //!
    //!
    FTFontChar(char charCode, int width, int height, int xOffset, int yOffset, int xAdvance, FT_GlyphRec_* glyph);

    //!
    //!
    //!
    ~FTFontChar();

    //!
    //!
    //!
    void calcVertices(int x, int y, TexturedVertex* vertices) const;

    //!
    //!
    //!
    void drawToBitmap(unsigned char* data, int texWidth, int texHeight);

    //!
    //!
    //!
    void releaseGlyph();

    //!
    //!
    //!
    void setXY(int x, int y);

    //!
    //!
    //!
    int getXAdvance();

    //!
    //!
    //!
    int getNumPixels() const;

    //!
    //!
    //!
    bool isEmpty() const;

    //!
    //!
    //!
    int getWidth();

    //!
    //!
    //!
    int getHeight();

private:

    //!
    //!
    //!
    void initTexCoords(int texWidth, int texHeight);

private:
    int x;
    int y;
    int charCode;
    int width;
    int height;
    int xOffset;
    int yOffset;
    int xAdvance;

    float texCoords[VERTICES_PER_QUAD * COMP_VERT_POS];

    struct FT_GlyphRec_* glyph;
};