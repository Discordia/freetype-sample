#pragma once

#include <string>
#include <vector>
#include <memory>
#include "FTFont.h"

using std::string;
using std::vector;
using std::shared_ptr;

struct FT_LibraryRec_;
class FTFont;
struct FTFontChar;

class FontAtlas
{
public:

    //!
    //!
    //!
    FontAtlas(int width, int height);

    //!
    //!
    //!
    ~FontAtlas();

    //!
    //!
    //!
    shared_ptr<FTFont> addFont(const string& fontName, unsigned int size, const string& letters);

    //!
    //!
    //!
    void create();

    //!
    //!
    //!
    unsigned int getTextureId();

private:

    //!
    //!
    //!
    void getNextTextureSize(int& texWidth, int& texHeight, int size);

    //!
    //!
    //!
    bool binPack(int texWidth, int texHeight);

private:

    //!
    int width;

    //!
    int height;

    //!
    unsigned int textureId;

    //!
    struct FT_LibraryRec_* library;

    //!
    vector<shared_ptr<FTFont> > fontList;

    //!
    vector<FTFontChar*> fontCharList;
};