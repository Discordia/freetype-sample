#pragma once

#include <Cache.h>
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
    static Cache<TreeNode>& getCache();

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
    static Cache<TreeNode> cache;

    int x;
    int y;
    int width;
    int height;
    TreeNode* leaf1;
    TreeNode* leaf2;
};
