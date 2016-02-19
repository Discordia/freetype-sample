#pragma once

#include <font/Pool.h>
#include "FTFontChar.h"

class FTFontChar;

class TreeNode
{
public:

    //!
    //!
    //!
    TreeNode();

    //!
    //!
    //!
    TreeNode(int x, int y, int width, int height);

    //!
    //!
    //!
    ~TreeNode();

    //!
    //!
    //!
    static Pool<TreeNode>& getPool();

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

    //!
    static Pool<TreeNode> pool;

    int x;
    int y;
    int width;
    int height;
    TreeNode* leaf1;
    TreeNode* leaf2;
};
