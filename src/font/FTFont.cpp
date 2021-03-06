#include <font/FTFontChar.h>
#include <font/FTFont.h>
#include <font/FontGeometry.h>

#include <ftimage.h>
#include <freetype.h>

#define LOG_TAG "FTFont"

FTFont::FTFont(FT_Face face, unsigned int textureId)
    : textureId(0)
{
    this->face = face;
    this->textureId = textureId;
    this->kerning = hasKerning(face);
}

FTFont::~FTFont()
{
    if (face)
    {
        FT_Done_Face(face);
        face = nullptr;
    }
}

shared_ptr<FontGeometry> FTFont::calcVertices(int x, int y, const string& text, int color, float alpha)
{
    unsigned long quads = text.length();
    TexturedVertex* vertices = new TexturedVertex[VERTICES_PER_QUAD * quads];

    unordered_map<uint32_t, FTFontChar*>::const_iterator it;

    int currX = x;
    FT_UInt glyph;
    FT_UInt glyphPrev = 0;

    for (int n = 0; text[n] != 0; n++)
    {
        uint32_t c = static_cast<uint32_t>(text[n]);
        it = fontCharList.find(c);

        if (it != fontCharList.end())
        {
            FTFontChar* fontChar = it->second;

            if (kerning)
            {
                glyph = FT_Get_Char_Index(face, c);
                currX += getKerning(glyphPrev, glyph);
                glyphPrev = glyph;
            }

            if (!fontChar->isEmpty())
            {
                fontChar->calcVertices(currX, y, &vertices[n * VERTICES_PER_QUAD]);
            }

            currX += fontChar->getXAdvance();
        }
    }

    return shared_ptr<FontGeometry>(new FontGeometry(textureId, color, alpha, quads, vertices));
}

void FTFont::addChar(uint32_t charCode, FTFontChar* fontChar)
{
    fontCharList[charCode] = fontChar;
}

void* FTFont::getChar(uint32_t charCode)
{
    unordered_map<uint32_t, FTFontChar*>::const_iterator it = fontCharList.find(charCode);

    if (it != fontCharList.end())
    {
        return it->second;
    }

    return nullptr;
}

bool FTFont::hasKerning(FT_Face face)
{
    bool hasKerning = false;

    if (face)
    {
        hasKerning = static_cast<bool >(FT_HAS_KERNING(face));
    }

    return hasKerning;
}

int FTFont::getKerning(unsigned int glyphPrev, unsigned int glyph)
{
    FT_Vector kerningVec;
    int kerningX = 0;

    if (glyphPrev && glyph)
    {
        FT_Get_Kerning(face, glyphPrev, glyph, FT_KERNING_DEFAULT, &kerningVec);
        kerningX += kerningVec.x >> 6;
    }

    return kerningX;
}
