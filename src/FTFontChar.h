#pragma once

struct FT_GlyphRec_;

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
    void render() const;

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
    void setGlyph(FT_GlyphRec_* glyph);

    //!
    //!
    //!
    int getNumPixels() const;

    //!
    //!
    //!
    bool isEmpty();

    //!
    //!
    //!
    int getWidth();

    //!
    //!
    //!
    int getHeight();

private:
    int x;
    int y;
    int charCode;
    int width;
    int height;
    int xOffset;
    int yOffset;
    int xAdvance;

    struct FT_GlyphRec_* glyph;
};