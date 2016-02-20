#pragma once

#include <memory>
#include <string>
#include <unordered_map>

using std::shared_ptr;
using std::string;
using std::unordered_map;

struct FT_FaceRec_;
class FTFontChar;
class FontBatchRenderer;
struct TexturedVertex;
class FontGeometry;

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
    shared_ptr<FontGeometry> calcVertices(int x, int y, const string& text, int color, float alpha);

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