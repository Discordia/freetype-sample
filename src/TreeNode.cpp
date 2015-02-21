#include <TreeNode.h>
#include <FTFontChar.h>

Pool<TreeNode> TreeNode::pool;

TreeNode::TreeNode()
    : leaf1(nullptr), leaf2(nullptr), x(0), y(0), width(0), height(0)
{
}

TreeNode::TreeNode(int x, int y, int width, int height)
    : leaf1(nullptr), leaf2(nullptr), x(x), y(y), width(width), height(height)
{
}

TreeNode::~TreeNode()
{
    // TODO: implement destruction
}

Pool<TreeNode>& TreeNode::getPool()
{
    return pool;
}

bool TreeNode::isEmpty() const
{
    return leaf1 == NULL && leaf2 == NULL;
}

void TreeNode::set(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->leaf1 = NULL;
    this->leaf2 = NULL;
}

bool TreeNode::add(FTFontChar* fontChar)
{
    if (fontChar->isEmpty()) return true;
    if (isEmpty())
    {
        if (fits(fontChar))
        {
            createBranches(fontChar);
            fontChar->setXY(x, y);

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

bool TreeNode::fits(FTFontChar *fontChar)
{
    return fontChar->getWidth() <= width && fontChar->getHeight() <= height;
}

void TreeNode::createBranches(FTFontChar *fontChar)
{
    int dx = width - fontChar->getWidth();
    int dy = height - fontChar->getHeight();

    // we split to give one very small leaf and one very big one
    // because it allows more efficient use of space
    // if you don't do this, the bottom right corner never gets used
    if (dx < dy)
    {
        //	split so the top is cut in half and the rest is one big rect below
        leaf1 = pool.allocate();
        leaf1->set(x + fontChar->getWidth(), y, width - fontChar->getWidth(), fontChar->getHeight());

        leaf2 = pool.allocate();
        leaf2->set(x, y + fontChar->getHeight(), width, height - fontChar->getHeight());
    }
    else
    {
        //	m_pLeaf1 = left (cut in half)
        leaf1 = pool.allocate();
        leaf1->set(x, y + fontChar->getHeight(), fontChar->getWidth(), height - fontChar->getHeight());

        // m_pLeaf2 = right (not cut)
        leaf2 = pool.allocate();
        leaf2->set(x + fontChar->getWidth(), y, width - fontChar->getWidth(), height);
    }
}
