#include <font/BinPacker.h>
#include <font/FTFontChar.h>

BinPacker::BinPacker(const Dimension& size)
    : size(size)
{
    root = new BinPackNode();
}

BinPacker::~BinPacker()
{
    delete root;
}

bool BinPacker::pack(vector<FTFontChar*> fontCharList)
{
    root->set(0, 0, size.width, size.height);
    for (int n = 0; n < (int) fontCharList.size(); n++)
    {
        if (!root->add(fontCharList[n]))
        {
            return false;
        }
    }

    return true;
}


///////////////////////////////////////////////////////////////
// BinPackNode implementation
///////////////////////////////////////////////////////////////

BinPackNode::BinPackNode()
    : leaf1(nullptr), leaf2(nullptr), x(0), y(0), width(0), height(0)
{
}

BinPackNode::BinPackNode(int x, int y, int width, int height)
        : leaf1(nullptr), leaf2(nullptr), x(x), y(y), width(width), height(height)
{
}

BinPackNode::~BinPackNode()
{
    if (leaf1 != nullptr)
    {
        delete leaf1;
    }

    if (leaf2 != nullptr)
    {
        delete leaf2;
    }
}

bool BinPackNode::isEmpty() const
{
    return leaf1 == nullptr && leaf2 == nullptr;
}

void BinPackNode::set(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->leaf1 = nullptr;
    this->leaf2 = nullptr;
}

bool BinPackNode::add(FTFontChar* fontChar)
{
    if (fontChar->isEmpty()) return true;
    if (isEmpty())
    {
        if (fits(fontChar))
        {
            createBranches(fontChar);
            fontChar->setXY(x + 1, y + 1);

            return true;
        }

        return false;
    }
    if (leaf1->add(fontChar))
    {
        return true;
    }

    return leaf2->add(fontChar);
}

bool BinPackNode::fits(FTFontChar *fontChar)
{
    return fontChar->getWidth() <= width && fontChar->getHeight() <= height;
}

void BinPackNode::createBranches(FTFontChar *fontChar)
{
    int dx = width - fontChar->getWidth();
    int dy = height - fontChar->getHeight();

    // we split to give one very small leaf and one very big one
    // because it allows more efficient use of space
    // if you don't do this, the bottom right corner never gets used
    if (dx < dy)
    {
        //	split so the top is cut in half and the rest is one big rect below
        leaf1 = new BinPackNode();
        leaf1->set(x + fontChar->getWidth() + 2, y, width - (fontChar->getWidth() + 2), fontChar->getHeight() + 2);

        leaf2 = new BinPackNode();
        leaf2->set(x, y + fontChar->getHeight() + 2, width, height - (fontChar->getHeight() + 2));
    }
    else
    {
        //	m_pLeaf1 = left (cut in half)
        leaf1 = new BinPackNode();
        leaf1->set(x, y + fontChar->getHeight() + 2, fontChar->getWidth() + 2, height - (fontChar->getHeight() + 2));

        // m_pLeaf2 = right (not cut)
        leaf2 = new BinPackNode();
        leaf2->set(x + fontChar->getWidth() + 2, y, width - (fontChar->getWidth() + 2), height);
    }
}
