#pragma once

#include <string>
#include <unordered_map>


using std::string;
using std::unordered_map;

struct FT_FaceRec_;
class FTFontChar;
class FontBatchRenderer;

class FTFont
{
public:

    //!
    //!
    //!
    FTFont(FT_FaceRec_* face, unsigned int textureId);

    //!
    //!
    //!
    ~FTFont();

    //!
    //!
    //!
    int drawString(int x, int y, const string& text, int color, float alpha = 1.0f);

    //!
    //! TODO: could this not be part of the constructor?
    //!
    void addChar(char chartCode, FTFontChar* fontChar);

    //!
    //!
    //!
    void* getChar(char i);

    //!
    //!
    //!
    int getTotalNumPixels();

    //!
    //!
    //!
    FontBatchRenderer& getRenderer();

private:

    //!
    //!
    //!
    bool hasKerning(FT_FaceRec_* face);

    //!
    //!
    //!
    int getKerning(unsigned int glyphPrev, unsigned int glyph);

private:

    //!
    unsigned int textureId;

    //!
    struct FT_FaceRec_* face;

    //!
    bool kerning;

    //!
    unordered_map<int, FTFontChar*> fontCharList;
};