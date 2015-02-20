Some thoughs on how the codebase should change.

* Need to fix so that the rendering is done according to freetypes model:
  http://www.freetype.org/freetype2/docs/tutorial/step2.html
   --> Kinda DONE, but should start to use advance when available

* useKerning should be per font and not per font atlas.
   => Then  we can work away so that FTFont foes not ahve to know about it's atlas.
   => FTFont need to know about it's own texture id.

* The whole rendering pipeline is messy:
  - font::drawString -> FTFont::render -> FTFontChar::render -> FontBatchRenderer::addQuad
  - Then we have to explicitly call FontBatchRenderer::render to render (if there are no render state changes).
   ==> Though on how it could be done:
    - Build the vertices for the whole stirng, since we know render state is not going to change
    - Cache render jobs (we will probably render them in the next frame also)
    - Batch everything up, sort by render state and render on end of frame

* Fix build for android with freetype

* Implement IOS support