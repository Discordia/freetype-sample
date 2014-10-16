#pragma once

struct FT_GlyphRec_;
class FontBatchRenderer;

const int VERTICES_PER_QUAD = 4;
const int INDICES_PER_QUAD = 6;
const int COMP_VERT_POS = 2;
const int COMP_VERT_TEX = 2;

class FTFontChar
{
public:

    //!
    //!
    //!
    FTFontChar();

    //!
    //!
    //!
    FTFontChar(char i);

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
    void setOffsets(int xOffset, int yOffset);

    //!
    //!
    //!
    void setSize(int width, int height);

    //!
    //!
    //!
    void setXAdvance(int xAdvance);

    //!
    //!
    //!
    int getXAdvance();

    //!
    //!
    //!
    void setGlyph(FT_GlyphRec_* glyph);

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