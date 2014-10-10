#pragma once

class FT_LibraryRec_;

class FTFont
{
    FTFont();

private:
    struct FT_LibraryRec_* library;
};