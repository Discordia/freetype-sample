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

FontAtlas::FontAtlas(bool useKerning)
        : useKerning(useKerning)
{
    if (FT_Init_FreeType(&library))
    {
        LOGE("Error inializing Freetype2");
    }
}

FontAtlas::~FontAtlas()
{
    if (library != nullptr)
    {
        FT_Done_FreeType(library);
        library = nullptr;
    }
}

void FontAtlas::addFont(const string& fontName, unsigned int size, const string& letters)
{
    // The Object In Which FreeType Holds Information On A Given
    // Font Is Called A "face".
    FT_Face face;

    string file = "assets/" + fontName;
    // This Is Where We Load In The Font Information From The File.
    if (FT_New_Face(library, file.c_str(), 0, &face))
    {
        // error
    }

    // FreeType Measures Font Size In Terms Of 1/64ths Of Pixels.
    FT_Set_Char_Size(face, size * 64, size * 64, 72, 72);

    unsigned long len = letters.size();
    int n;
    FTFontChar* fontChar;
    FT_Glyph pGlyph;
    FTFont* font = new FTFont(this);
    font->setLineHeight(face->size->metrics.height >> 6);
    font->setFTFace(face);
    fontList.push_back(font);

    char c;
    long height;
    long yOffset;
    int ixGlyph;
    for (n = 0; n < len; n++)
    {
        c = letters[n];

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
                if (FT_Load_Glyph(face, ixGlyph, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT))
                {
                    // Error
                }

                // Move The Face's Glyph Into A Glyph Object.
                if (FT_Get_Glyph(face->glyph, &pGlyph))
                {
                    // Error
                }

                fontChar = new FTFontChar(c);

                // all metrics dimensions are multiplied by 64, so we have to divide by 64
                height = face->glyph->metrics.height >> 6;
                yOffset = font->getLineHeight() - (face->glyph->metrics.horiBearingY >> 6);
                fontChar->setOffsets(face->glyph->metrics.horiBearingX >> 6, yOffset);
                fontChar->setSize(face->glyph->metrics.width >> 6, height);
                fontChar->setXAdvance(face->glyph->metrics.horiAdvance >> 6);
                fontChar->setGlyph(pGlyph);
                fontCharList.push_back(fontChar);
                font->addChar(c, fontChar);
            }
        }
    }
}

bool greaterSizeComparator(FTFontChar* fontChar1, FTFontChar* fontChar2)
{
    return fontChar1->getNumPixels() > fontChar2->getNumPixels();
}

void FontAtlas::create()
{
    int n;
    int totalPixels = 0;
    for (n = 0; n < (int) fontCharList.size(); n++)
    {
        totalPixels += fontCharList[n]->getNumPixels();
    }

    int ixSize = 0;
    int texWidth = 32;
    int texHeight = 32;
    while (true)
    {
        if (totalPixels <= texWidth * texHeight)
        {
            break;
        }
        getNextTextureSize(texWidth, texHeight, ixSize);
        ixSize++;
    }
    sort(fontCharList.begin(), fontCharList.end(), greaterSizeComparator);
    TreeNode::getCache().init((int) (fontCharList.size() + 1) * 2);
    while (!binPack(texWidth, texHeight))
    {
        TreeNode::getCache().destroy();
        getNextTextureSize(texWidth, texHeight, ixSize);
        ixSize++;
    }
    TreeNode::getCache().release();

    unsigned char* data = new unsigned char[texWidth * texHeight];
    for (n = 0; n < (int) fontCharList.size(); n++)
    {
        fontCharList[n]->drawToBitmap(data, texWidth, texHeight);
        fontCharList[n]->releaseGlyph();
    }

    for (n = 0; n < (int) fontList.size(); n++)
    {
        fontList[n]->finishCreating();
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texWidth, texHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    int err = glGetError();
    if (err != GL_NO_ERROR)
    {
        LOGE("Error in glTexImage2D: %i", err);
    }

    // clean up memory
    delete[] data;
}

bool FontAtlas::isUseKerning()
{
    return useKerning;
}

void FontAtlas::getNextTextureSize(int& texWidth, int& texHeight, int size)
{
    texHeight *= 2;
    texWidth *= 2;
    if (texWidth > 1024 || texHeight > 1024)
    {
        LOGE("To many images to fit in one texture");
    }
}

bool FontAtlas::binPack(int texWidth, int texHeight)
{
    TreeNode* treeNode = TreeNode::getCache().allocate();
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

unsigned int FontAtlas::getTextureId()
{
    return textureId;
}
