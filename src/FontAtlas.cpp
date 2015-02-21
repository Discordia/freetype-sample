#include <FontAtlas.h>

#include <FTFontChar.h>
#include <FTFont.h>
#include <TreeNode.h>
#include <OpenGL.h>
#include <Log.h>

#include <ft2build.h>
#include <ftglyph.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <freetype.h>

using std::sort;

#define LOG_TAG "FontAtlas"

FontAtlas::FontAtlas(int width, int height)
{
    this->width = width;
    this->height = height;

    if (FT_Init_FreeType(&library))
    {
        LOGE("Error inializing Freetype2");
    }

    glGenTextures(1, &textureId);
}

FontAtlas::~FontAtlas()
{
    if (library != nullptr)
    {
        FT_Done_FreeType(library);
        library = nullptr;
    }
}

shared_ptr<FTFont> FontAtlas::addFont(const string& fontName, unsigned int size, const string& letters)
{
    // The Object In Which FreeType Holds Information On A Given
    // Font Is Called A "face".
    FT_Face face;

    string file = "assets/" + fontName;
    // This Is Where We Load In The Font Information From The File.
    if (FT_Error error = FT_New_Face(library, file.c_str(), 0, &face))
    {
        LOGE("Failed to load font.");
        exit(1);
    }

    // FreeType Measures Font Size In Terms Of 1/64ths Of Pixels.
    FT_Set_Char_Size(face, size * 64, size * 64, 72, 72);

    unsigned long len = letters.size();

    shared_ptr<FTFont> font = shared_ptr<FTFont>(new FTFont(face, textureId));
    fontList.push_back(font);

    FTFontChar* fontChar;
    FT_Glyph pGlyph;
    int ixGlyph;

    for (int n = 0; n < len; n++)
    {
        char c = letters[n];

        // check that the character hasn't already been processed
        if (font->getChar(c) == NULL)
        {
            // Load The Glyph For Our Character.
            ixGlyph = FT_Get_Char_Index(face, c);
            if (ixGlyph == 0)
            {
                LOGE("character doesn't exist in font: %c", c);
            }
            else
            {
                if (FT_Load_Glyph(face, ixGlyph, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_NORMAL))
                {
                    LOGE("Failed to load the glyph for char c=%c.", c);
                }

                // Move The Face's Glyph Into A Glyph Object.
                if (FT_Get_Glyph(face->glyph, &pGlyph))
                {
                    LOGE("Failed to load the glyph object for char c=%c.", c);
                }

                fontChar = new FTFontChar(c);

                // all metrics dimensions are multiplied by 64, so we have to divide by 64
                fontChar->setOffsets(face->glyph->metrics.horiBearingX >> 6, face->glyph->metrics.horiBearingY >> 6);
                fontChar->setSize(face->glyph->metrics.width >> 6, face->glyph->metrics.height >> 6);
                fontChar->setXAdvance(face->glyph->metrics.horiAdvance >> 6);
                fontChar->setGlyph(pGlyph);
                fontCharList.push_back(fontChar);
                font->addChar(c, fontChar);
            }
        }
    }

    return font;
}

bool greaterSizeComparator(FTFontChar* fontChar1, FTFontChar* fontChar2)
{
    return fontChar1->getNumPixels() > fontChar2->getNumPixels();
}

void FontAtlas::create()
{
    int totalPixels = 0;
    for (int n = 0; n < (int) fontList.size(); n++)
    {
        shared_ptr<FTFont> font = fontList[n];
        totalPixels += font->getTotalNumPixels();
    }

    sort(fontCharList.begin(), fontCharList.end(), greaterSizeComparator);

    TreeNode::getPool().init((int) (fontCharList.size() + 1) * 2);
    if (!binPack(width, height))
    {
        LOGE("Failed to render glyphs to texture");
    }
    TreeNode::getPool().release();

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    int err = glGetError();
    if (err != GL_NO_ERROR)
    {
        LOGE("Error in glTexImage2D: %i", err);
    }

    for (int n = 0; n < (int) fontList.size(); n++)
    {
        fontList[n]->finishCreating();
    }

    // clean up memory
    delete[] data;
}

bool FontAtlas::binPack(int texWidth, int texHeight)
{
    TreeNode* treeNode = TreeNode::getPool().allocate();
    treeNode->set(0, 0, texWidth, texHeight);
    for (int n = 0; n < (int) fontCharList.size(); n++)
    {
        if (!treeNode->add(fontCharList[n]))
        {
            return false;
        }
    }
    return true;
}