#include <FTFontChar.h>
#include <FTFont.h>
#include <FontBatchRenderer.h>

#include <ftimage.h>
#include <freetype.h>

#define LOG_TAG "FTFont"

FTFont::FTFont(FT_FaceRec_* face, unsigned int textureId)
    : textureId(0)
{
    this->face = face;
    this->textureId = textureId;
}

FTFont::~FTFont()
{
    releaseFace();
}

int FTFont::drawString(int x, int y, const string& text, int color, float alpha)
{
    getRenderer().setAttributes(textureId, color, alpha);

    unsigned char c;
    int currX = x;
    int n = 0;

    unordered_map<int, FTFontChar*>::const_iterator it;

    FT_Vector kerning;
    FT_UInt ixGlyph;
    FT_UInt ixGlyphPrev = 0;

    bool fontHasKerning = hasKerning();

    // TODO: iterate over string the c++ way instead of c way
    while (text[n] != 0)
    {
        c = (unsigned char) text[n];

        it = fontCharList.find(c);
        if (it != fontCharList.end())
        {
            if (fontHasKerning)
            {
                // get kerning
                ixGlyph = FT_Get_Char_Index(face, c);
                if (ixGlyphPrev && ixGlyph)
                {
                    FT_Get_Kerning(face, ixGlyphPrev, ixGlyph, FT_KERNING_DEFAULT, &kerning);
                    currX += kerning.x >> 6;
                }

                ixGlyphPrev = ixGlyph;
            }

            it->second->render(currX, y);
            currX += it->second->getXAdvance();
        }

        n++;
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

void FTFont::finishCreating()
{
    if (!hasKerning())
    {
        releaseFace();
    }
}

void FTFont::releaseFace()
{
    if (face)
    {
        FT_Done_Face(face);
        face = nullptr;
    }
}

bool FTFont::hasKerning()
{
    bool hasKerning = false;

    if (face)
    {
        hasKerning = static_cast<bool >(FT_HAS_KERNING(face));
    }

    return hasKerning;
}

FontBatchRenderer& FTFont::getRenderer()
{
    return FontBatchRenderer::getRenderer();
}
