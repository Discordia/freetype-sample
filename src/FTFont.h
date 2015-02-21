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
    //! TODO: Is this needed?
    //!
    void finishCreating();

    //!
    //!
    //!
    FontBatchRenderer& getRenderer();

private:

    //!
    //!
    //!
    void releaseFace();

    //!
    //!
    //!
    bool hasKerning();

private:

    //!
    unsigned int textureId;

    //!
    struct FT_FaceRec_* face;

    //!
    unordered_map<int, FTFontChar*> fontCharList;
};