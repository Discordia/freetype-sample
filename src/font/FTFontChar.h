#pragma once

struct FT_GlyphRec_;
class FontBatchRenderer;

const int VERTICES_PER_QUAD = 4;
const int INDICES_PER_QUAD = 6;
const int COMP_VERT_POS = 2;
const int COMP_VERT_TEX = 2;

const int WHITE = 0xffffff;
const int BLACK = 0;

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
    void render(int x, int y) const;

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

    //!
    //!
    //!
    FontBatchRenderer& getRenderer() const;


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