#pragma once

class FT_LibraryRec_;

class FTFont
{
public:
    FTFont();
    ~FTFont();

private:
    struct FT_LibraryRec_* library;
};