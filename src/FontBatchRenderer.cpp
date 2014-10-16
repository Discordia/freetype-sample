#include "FontBatchRenderer.h"

FontBatchRenderer FontBatchRenderer::renderer;

FontBatchRenderer::FontBatchRenderer()
{

}

FontBatchRenderer::~FontBatchRenderer()
{

}

FontBatchRenderer& FontBatchRenderer::getRenderer()
{
    return renderer;
}

void FontBatchRenderer::setAttributes(unsigned int textureId, int color, float alpha, bool transparent, bool smooth)
{

}

void FontBatchRenderer::addQuad(const float* texCoords, const float* vertices)
{

}
