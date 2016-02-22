#pragma once

#include <vector>
#include <core/Dimension.h>

using std::vector;

class BinPackNode;
class FTFontChar;

class BinPacker
{
public:
    BinPacker(const Dimension& size);
    ~BinPacker();

    bool pack(vector<FTFontChar*> fontCharList);

private:
    Dimension size;
    BinPackNode* root;
};

class BinPackNode
{
public:

    //!
    //!
    //!
    BinPackNode();

    //!
    //!
    //!
    BinPackNode(int x, int y, int width, int height);

    //!
    //!
    //!
    ~BinPackNode();

    //!
    //!
    //!
    bool isEmpty() const;

    //!
    //!
    //!
    void set(int x, int y, int width, int height);

    //!
    //!
    //!
    bool add(FTFontChar* fontChar);

private:

    //!
    //!
    //!
    bool fits(FTFontChar *fontChar);

    //!
    //!
    //!
    void createBranches(FTFontChar *fontChar);

private:
    int x;
    int y;
    int width;
    int height;
    BinPackNode* leaf1;
    BinPackNode* leaf2;
};