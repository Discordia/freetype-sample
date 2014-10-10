#include <FTFont.h>
#include "Log.h"

#include <ft2build.h>
#include  FT_FREETYPE_H

#define LOG_TAG "FTFont"


FTFont::FTFont()
{
    FT_Error error = FT_Init_FreeType(&library);
    if (error)
    {
        LOGE("Error inializing Freetype2");
    }
}

FTFont::~FTFont()
{
    FT_Done_FreeType(library);
}