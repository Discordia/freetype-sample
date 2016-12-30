#include <font/FTFontChar.h>

#include <cassert>
#include <ftglyph.h>

FTFontChar::FTFontChar(char charCode, int width, int height, int xOffset, int yOffset, int xAdvance, FT_GlyphRec_* glyph)
    : x(0), y(0), charCode(charCode), width(width), height(height), xOffset(xOffset), yOffset(yOffset), xAdvance(xAdvance), glyph(glyph)
{
}

FTFontChar::~FTFontChar()
{
}

void FTFontChar::calcVertices(int x, int y, TexturedVertex* vertices) const
{
    int xo = x + xOffset;

    vertices[0].x  = (float) xo;
    vertices[0].y  = (float) (y + yOffset);
    vertices[0].u  = texCoords[0];
    vertices[0].v  = texCoords[1];

    vertices[1].x  = (float) xo;
    vertices[1].y  = (float) y - (height - yOffset);
    vertices[1].u  = texCoords[2];
    vertices[1].v  = texCoords[3];

    vertices[2].x  = (float) (xo + width);
    vertices[2].y  = (float) y - (height - yOffset);
    vertices[2].u  = texCoords[4];
    vertices[2].v  = texCoords[5];

    vertices[3].x  = (float) (xo + width);
    vertices[3].y  = (float) (y + yOffset);
    vertices[3].u  = texCoords[6];
    vertices[3].v  = texCoords[7];
}

void FTFontChar::drawToBitmap(unsigned char* data, int texWidth, int texHeight)
{
    if (isEmpty())
    {
        return;
    }

    initTexCoords(texWidth, texHeight);

    // Convert The Glyph To A Bitmap.
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

    // This Reference Will Make Accessing The Bitmap Easier.
    FT_Bitmap& bitmap = bitmap_glyph->bitmap;

    assert(bitmap.width == width);
    assert(bitmap.rows == height);

    uint32_t x, y = 0;
    int index;

    for (y = 0; y < bitmap.rows; y++)
    {
        for (x = 0; x < bitmap.width; x++)
        {
            index = (this->y + y) * texWidth + this->x + x;
            data[index] = bitmap.buffer[y * bitmap.width + x];
        }
    }
}

void FTFontChar::initTexCoords(int texWidth, int texHeight)
{
    float x1 = (float) this->x / (float) texWidth;
    float y1 = (float) this->y / (float) texHeight;
    float x2 = (float)(this->x + this->width) / (float) texWidth;
    float y2 = (float)(this->y + this->height) / (float) texHeight;

    texCoords[0] = x1;
    texCoords[1] = y1;

    texCoords[2] = x1;
    texCoords[3] = y2;

    texCoords[4] = x2;
    texCoords[5] = y2;

    texCoords[6] = x2;
    texCoords[7] = y1;
}

void FTFontChar::releaseGlyph()
{
    if (glyph)
    {
        FT_Done_Glyph(glyph);
    }
}

void FTFontChar::setXY(int x, int y)
{
    this->x = x;
    this->y = y;
}

int FTFontChar::getXAdvance()
{
    return xAdvance;
}

int FTFontChar::getNumPixels() const
{
    return width * height;
}

bool FTFontChar::isEmpty() const
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