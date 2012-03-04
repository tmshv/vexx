#ifndef SYVIEWER_H
#define SYVIEWER_H

#include "UISurface.h"
#include "saviewport.h"
#include "sentityweakpointer.h"

class SyViewer : public SViewport
  {
  Q_OBJECT

public:
  SyViewer();

  Q_INVOKABLE void setInput(SProperty *vp);

private:
  SEntityWeakPointer _vp;
  };

#endif // SYVIEWER_H
