#include <FTFontChar.h>
#include <FontBatchRenderer.h>

#include <cassert>

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

void FTFontChar::render(int x, int y) const
{
    if (isEmpty())
    {
        return;
    }

    x += xOffset;

    float vertices[VERTICES_PER_QUAD * (COMP_VERT_POS + 1)];

    vertices[0]  = (float) x;
    vertices[1]  = (float) (y + yOffset);
    vertices[2]  = 0.3f;

    vertices[3]  = (float) x;
    vertices[4]  = (float) y - (height - yOffset);
    vertices[5]  = 0.3f;

    vertices[6]  = (float) (x + width);
    vertices[7]  = (float) y - (height - yOffset);
    vertices[8]  = 0.3f;

    vertices[9]  = (float) (x + width);
    vertices[10] = (float) (y + yOffset);
    vertices[11] = 0.3f;

    getRenderer().addQuad(vertices, texCoords);
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

    int x, y = 0;
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

int FTFontChar::getXAdvance()
{
    return xAdvance;
}

void FTFontChar::setGlyph(FT_GlyphRec_* glyph)
{
    this->glyph = glyph;
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

FontBatchRenderer& FTFontChar::getRenderer() const
{
    return FontBatchRenderer::getRenderer();
}
