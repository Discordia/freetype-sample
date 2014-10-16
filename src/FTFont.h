#pragma once

struct FT_FaceRec_;
class FTFontChar;
class FontAtlas;

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

    void finishCreating();

    void setLineHeight(long lineHeight);

    void setFTFace(FT_FaceRec_* ftFace);
    void* getChar(char i);
    long getLineHeight();
    void addChar(char i, FTFontChar *fontChar);
};