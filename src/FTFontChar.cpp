#include <FTFontChar.h>
#include <stdio.h>
#include <ftglyph.h>

FTFontChar::FTFontChar()
        : x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0), glyph(NULL)
{
}

FTFontChar::FTFontChar(char charCode)
        : charCode(charCode)
{
}

FTFontChar::~FTFontChar()
{
}

void FTFontChar::render() const
{

}

void FTFontChar::setXY(int x, int y)
{
    this->x = x;
    this->y = y;
}

void FTFontChar::setOffsets(int xOffset, int yOffset)
{
    this->xOffset = xOffset;
    this->yOffset = yOffset;
}

void FTFontChar::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void FTFontChar::setXAdvance(int xAdvance)
{
    this->xAdvance = xAdvance;
}

void FTFontChar::setGlyph(FT_GlyphRec_* glyph)
{
    this->glyph = glyph;
}

int FTFontChar::getNumPixels() const
{
    return width * height;
}

bool FTFontChar::isEmpty()
{
    return width == 0 || height == 0;
}

int FTFontChar::getWidth()
{
    return width;
}

int FTFontChar::getHeight()
{
    return height;
}
