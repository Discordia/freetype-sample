Some thoughts on how the codebase should change.

* fontCharList is stored both in FontAtlas and FTFont.
  ==> It should only be needed at one of the places, probably only FTFont.

* The whole rendering pipeline is messy:
  - font::drawString -> FTFont::render -> FTFontChar::render -> FontBatchRenderer::addQuad
  - Then we have to explicitly call FontBatchRenderer::render to render (if there are no render state changes).
   ==> Though on how it could be done:
    - Build the vertices for the whole stirng, since we know render state is not going to change
    - Cache render jobs (we will probably render them in the next frame also)
    - Batch everything up, sort by render state and render on end of frame

* Fix build for android with freetype

* Implement IOS support