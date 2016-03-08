#include <font/FontAtlas.h>

#include <core/Log.h>
#include <core/OpenGL.h>
#include <core/StreamFactory.h>

#include <font/FTFontChar.h>
#include <font/FTFont.h>
#include <font/BinPacker.h>

#include <ft2build.h>
#include <ftglyph.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <freetype.h>


using std::sort;

#define LOG_TAG "FontAtlas"

FontAtlas::FontAtlas(shared_ptr<StreamFactory> streamFactory, int width, int height)
    : streamFactory(streamFactory)
{
    this->width = width;
    this->height = height;

    if (FT_Init_FreeType(&library))
    {
        LOGE("Error initializing Freetype2");
    }

    glGenTextures(1, &textureId);
}

FontAtlas::~FontAtlas()
{
    fontFiles.clear();
    fontList.clear();

    for (auto it = fontCharList.begin(); it != fontCharList.end(); ++it)
    {
        delete *it;
    }
    fontCharList.clear();

    if (library)
    {
        FT_Done_FreeType(library);
        library = nullptr;
    }
}

weak_ptr<FTFont> FontAtlas::addFont(const string& fontName, unsigned int size, const string& letters)
{
    FT_Face face;

    shared_ptr<Stream> fontFile = streamFactory->open(fontName);
    fontFiles.push_back(fontFile);
    char* fontData = new char[fontFile->size()];
    fontFile->read(fontData, fontFile->size());

    if (FT_Error error = FT_New_Memory_Face(library, (FT_Byte*) fontData, fontFile->size(), 0, &face))
    {
        LOGE("Failed to load font.");
        return shared_ptr<FTFont>();
    }

    // FreeType Measures Font Size In Terms Of 1/64ths Of Pixels.
    FT_Set_Char_Size(face, size * 64, size * 64, 72, 72);

    shared_ptr<FTFont> font = shared_ptr<FTFont>(new FTFont(face, textureId));
    fontList.push_back(font);

    FT_Glyph pGlyph;
    unsigned int ixGlyph;

    for (int n = 0; n < letters.size(); n++)
    {
        uint32_t c = static_cast<uint32_t>(letters[n]);
        if (font->getChar(c) != nullptr)
        {
            return shared_ptr<FTFont>();
        }

        ixGlyph = FT_Get_Char_Index(face, c);
        if (ixGlyph == 0)
        {
            LOGE("character doesn't exist in font: %c", c);
            return shared_ptr<FTFont>();
        }

        if (FT_Load_Glyph(face, ixGlyph, FT_LOAD_RENDER))
        {
            LOGE("Failed to load the glyph for char c=%c.", c);
            return shared_ptr<FTFont>();
        }

        // Move The Face's Glyph Into A Glyph Object.
        if (FT_Get_Glyph(face->glyph, &pGlyph))
        {
            LOGE("Failed to load the glyph object for char c=%c.", c);
            return shared_ptr<FTFont>();
        }

        // all metrics dimensions are multiplied by 64, so we have to divide by 64
        int xOffset = (int) face->glyph->metrics.horiBearingX >> 6;
        int yOffset = (int) face->glyph->metrics.horiBearingY >> 6;
        int width = (int) face->glyph->metrics.width >> 6;
        int height = (int) face->glyph->metrics.height >> 6;
        int xAdvance = (int) face->glyph->metrics.horiAdvance >> 6;

        // Create a new FTFontChar that represent one chartacter in the font
        FTFontChar* fontChar = new FTFontChar(c, width, height, xOffset, yOffset, xAdvance, pGlyph);
        fontCharList.push_back(fontChar);
        font->addChar(c, fontChar);
    }

    return font;
}

bool greaterSizeComparator(FTFontChar* fontChar1, FTFontChar* fontChar2)
{
    return fontChar1->getNumPixels() > fontChar2->getNumPixels();
}

void FontAtlas::create()
{
    sort(fontCharList.begin(), fontCharList.end(), greaterSizeComparator);

    BinPacker binPacker(Dimension(width, height));
    if (!binPacker.pack(fontCharList))
    {
        LOGE("Failed to render glyphs to texture. Glyphs did not fit.");
    }

    unsigned char* data = new unsigned char[width * height];
    for (int n = 0; n < (int) fontCharList.size(); n++)
    {
        fontCharList[n]->drawToBitmap(data, width, height);
        fontCharList[n]->releaseGlyph();
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    int err = glGetError();
    if (err != GL_NO_ERROR)
    {
        LOGE("Error in glTexImage2D: %i", err);
    }

    // clean up memory
    delete[] data;
}
