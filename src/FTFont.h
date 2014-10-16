#pragma once

#include <string>
#include <unordered_map>
#include "FontBatchRenderer.h"

using std::string;
using std::unordered_map;

struct FT_FaceRec_;
class FTFontChar;
class FontAtlas;
class FontBatchRenderer;

class FTFont
{
public:

    //!
    //!
    //!
    FTFont(FontAtlas *fontAtlas);

    //!
    //!
    //!
    ~FTFont();
    
    //!
    //!
    //!
    int drawString(int x, int y, const string& text, int color, float alpha = 1.0f);

    //!
    //!
    //!
    int getLineHeight();

    //!
    //! TODO: could this not be part of the constructor?
    //!
    void setLineHeight(int lineHeight);

    //!
    //! TODO: could this not be part of the constructor?
    //!
    void addChar(char chartCode, FTFontChar* fontChar);

    //!
    //!
    //!
    void* getChar(char i);

    //!
    //! TODO: could this not be part of the constructor?
    //!
    void setFTFace(FT_FaceRec_* ftFace);

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
    FontAtlas* fontAtlas;

    //!
    struct FT_FaceRec_* ftFace;

    //!
    int lineHeight;

    //!
    unordered_map<int, FTFontChar*> fontCharList;
};