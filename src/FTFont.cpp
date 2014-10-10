#include <FTFont.h>

#include <ft2build.h>
#include FT_FREETYPE_H

FTFont::FTFont()
{
    FT_Init_FreeType(&library);
}