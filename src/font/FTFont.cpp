#include <font/FTFontChar.h>
#include <font/FTFont.h>
#include <font/FontBatchRenderer.h>

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

int FTFont::drawString(int x, int y, const string& text, int color, float alpha)
{
    getRenderer().setAttributes(textureId, color, alpha);

    unordered_map<int, FTFontChar*>::const_iterator it;

    unsigned char c;
    int currX = x;
    FT_UInt glyph;
    FT_UInt glyphPrev = 0;

    for (int n = 0; text[n] != 0; n++)
    {
        c = (unsigned char) text[n];
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

            fontChar->render(currX, y);
            currX += fontChar->getXAdvance();
        }
    }

    return currX;
}

void FTFont::addChar(char charCode, FTFontChar* fontChar)
{
    fontCharList[charCode] = fontChar;
}

void* FTFont::getChar(char charCode)
{
    unordered_map<int, FTFontChar*>::const_iterator it = fontCharList.find(charCode);

    if (it != fontCharList.end())
    {
        return it->second;
    }

    return nullptr;
}

int FTFont::getTotalNumPixels()
{
    int totalPixels = 0;
    for (auto it = fontCharList.begin(); it != fontCharList.end(); ++it)
    {
        totalPixels += it->second->getNumPixels();
    }

    return totalPixels;
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

FontBatchRenderer& FTFont::getRenderer()
{
    return FontBatchRenderer::getRenderer();
}
