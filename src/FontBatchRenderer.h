#pragma once

class FontBatchRenderer
{
public:

    //!
    //!
    //!
    FontBatchRenderer();

    //!
    //!
    //!
    ~FontBatchRenderer();

    //!
    //!
    //!
    static FontBatchRenderer& getRenderer();

    //!
    //!
    //!
    void setAttributes(unsigned int textureId, int color, float alpha, bool transparent, bool smooth);

    //!
    //!
    //!
    void addQuad(const float* texCoords, const float* vertices);

private:

    //! Instance
    static FontBatchRenderer renderer;
};