#ifndef UIGRAPHDEBUGSURFACE_H
#define UIGRAPHDEBUGSURFACE_H

#include "UISurface.h"
#include "X2DCanvas.h"
#include "GCShiftRenderModel.h"
#include "XAbstractDelegate.h"
#include "XAbstractCanvasController.h"
#include "aplugin.h"
#include "splugin.h"
#include "QStaticText"
#include "QFontMetrics"
#include "GCAbstractNodeDelegate.h"
#include "GCNodeController.h"

class QMenu;
class UIPlugin;

class UIGraphDebugSurface : public QObject, public UISurface, public GCShiftRenderModel
  {
  Q_OBJECT
public:
  UIGraphDebugSurface(SEntity *ent);
  ~UIGraphDebugSurface();

  virtual const XAbstractDelegate *delegateFor(XAbstractRenderModel::Iterator *, const XAbstractCanvas *canvas) const;

private slots:
  void createEntity(QAction*);
  void createProperty(QAction*);
  void onContextMenu(QPoint);
  void disconnect();
  void deleteEntity();

private:

  void generatePropertyActions(const SPropertyInformation *mustInheritFrom, QMenu *menuToAdd);

  X2DCanvas *_canvas;

  XAbstractRenderModel::Iterator *_iterator;

  GCSimpleNodeDelegate _delegate;
  GCNodeController _controller;
  };

#endif // UIGRAPHDEBUGSURFACE_H
