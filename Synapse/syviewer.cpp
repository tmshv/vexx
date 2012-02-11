#include "syviewer.h"
#include "sypreviewviewport.h"
#include "syimageinput.h"

SyViewer::SyViewer() : SViewport(0)
  {
  }

void SyViewer::setInput(SProperty *vp)
  {
  SDatabase *db = vp->database();

  SBlock blk(vp->handler());
  if(_vp)
    {
    db->children.remove(_vp);
    }

  SyImageInput *input = vp->castTo<SyImageInput>();
  if(!input)
    {
    return;
    }

  XAbstractCanvasController *cntrl = 0;
  SyPreviewViewport *viewport = db->addChild<SyPreviewViewport>("");
  viewport->setupViewport(input, &cntrl);
  setViewport(viewport);
  setController(cntrl);
  }
