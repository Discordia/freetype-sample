#pragma once

#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;

struct FT_LibraryRec_;
class FTFont;
struct FTFontChar;
class Stream;
class StreamFactory;

class FontAtlas
{
public:

    //!
    //!
    //!
    FontAtlas(shared_ptr<StreamFactory> streamFactory, int width, int height);

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

    //!
    vector<shared_ptr<Stream>> fontFiles;

    //!
    shared_ptr<StreamFactory> streamFactory;
};