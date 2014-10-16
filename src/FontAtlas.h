#pragma once

#include <string>
#include <vector>
#include "FTFont.h"

using std::string;
using std::vector;

struct FT_LibraryRec_;
class FTFont;
struct FTFontChar;

class FontAtlas
{
public:

    //!
    //!
    //!
    FontAtlas();

    //!
    //!
    //!
    ~FontAtlas();

    //!
    //!
    //!
    void addFont(const string& fontName, unsigned int size, const string& letters);

    //!
    //!
    //!
    void create();

private:

    //!
    //!
    //!
    void getNextTextureSize(int& texWidth, int& texHeight, int size);

private:

    //!
    //!
    //!
    bool binPack(int texWidth, int texHeight);

private:

    //!
    struct FT_LibraryRec_* library;

    //!
    vector<FTFont*> fontList;

    //!
    vector<FTFontChar*> fontCharList;

    //!
    unsigned int textureId;
};