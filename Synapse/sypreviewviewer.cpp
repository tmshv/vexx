#include "sypreviewviewer.h"
#include "sproperty.h"
#include "syviewernode.h"
#include "QPainter"

SyPreviewViewer::SyPreviewViewer(SEntity *root) : UISurface("Preview", _canvas = new X2DCanvas(), UISurface::Dock),
  GCShiftRenderModel(0, SIterator::createFilter<ChildTree, Typed<SyViewerNode>::Is>()), _controller(_canvas)
  {
  _canvas->setModel(this);
  _canvas->setBackgroundColour(Qt::black);
  setEntity(root);
  }

const XAbstractDelegate *SyPreviewViewer::delegateFor(XAbstractRenderModel::Iterator *it, const XAbstractCanvas *) const
  {
  GCShiftRenderModel::Iterator *slIt = static_cast<GCShiftRenderModel::Iterator*>(it);
  xAssert(slIt->entity()->castTo<SyViewerNode>());
  return this;
  }

void SyPreviewViewer::paint(xuint32 pass, XAbstractCanvas *c, const XAbstractRenderModel::Iterator *it, const XAbstractRenderModel *) const
  {
  SProfileFunction
  const GCShiftRenderModel::Iterator *slIt = static_cast<const GCShiftRenderModel::Iterator*>(it);

  X2DCanvas* canvas = static_cast<X2DCanvas*>(c);
  QPainter *ptr = canvas->currentPainter();
  xAssert(ptr);

  SyViewerNode *viewerNode(slIt->entity()->castTo<SyViewerNode>());
  //ptr->drawImage(0, 0, viewerNode->preview());
  }
