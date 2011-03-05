#include "surface.h"
#include "QMoveEvent"
#ifdef Q_OS_WIN
# include "windows.h"
#endif
#include "QDebug"

Surface * Surface::_mainSurface = 0;
QList<Surface*> Surface::_surfaces;

Surface::Surface(QWidget *widg) : _widget(widg)
  {
  _previousHitMode = (quint32)-1;

  _surfaces << this;
  if(!_mainSurface)
    {
    _mainSurface = this;
    }
  else
    {
    setParent(_mainSurface);
    setWindowFlags(Qt::Tool);
    }
  }

Surface::~Surface()
  {
  _surfaces.removeAll(this);
  }

Surface *Surface::getMainWidget()
  {
  return _mainSurface;
  }

const Surface *Surface::getMainWidget() const
  {
  return _mainSurface;
  }

bool Surface::isMainSurface() const
  {
  return this == _mainSurface;
  }

#ifdef Q_OS_WIN
bool Surface::winEvent(MSG *message, long *result)
  {
  if(message->message == WM_EXITSIZEMOVE)
    {
    finaliseMove();
    }
  return false;
  }
#endif

void Surface::moveEvent(QMoveEvent *)
  {
  if(!isMainSurface())
    {
    QPoint globalPt = QCursor::pos();

    foreach(Surface *s, _surfaces)
      {
      if(s != this)
        {
        QPoint localPt = s->mapFromGlobal(globalPt);
        if(QRect(QPoint(), s->size()).contains(localPt))
          {
          _previousHitSurface = s;
          _previousHitMode = s->hitTest(localPt, this);
          break;
          }
        }
      }
    }
  }

void Surface::finaliseMove()
  {
  if(_previousHitMode != ((quint32)-1))
    {
    apply(_previousHitMode, _previousHitSurface);

    _previousHitMode = (quint32)-1;
    }
  }

quint32 Surface::hitTest(QPoint pt, Surface *) const
  {
  if(pt.y() < 20)
    {
    return HitTop;
    }
  if(pt.x() < 20)
    {
    return HitLeft;
    }
  if(pt.y() > (height()-20))
    {
    return HitBottom;
    }
  if(pt.x() > (width()-20))
    {
    return HitRight;
    }
  return (quint32)-1;
  }

void Surface::apply(quint32 mode, Surface *hitSurface)
  {
  if(mode == HitLeft)
    {
    hitSurface->dock(DockLeft, this);
    }
  if(mode == HitRight)
    {
    hitSurface->dock(DockRight, this);
    }
  if(mode == HitTop)
    {
    hitSurface->dock(DockTop, this);
    }
  if(mode == HitBottom)
    {
    hitSurface->dock(DockBottom, this);
    }
  }

void Surface::dock(DockModemode, Surface *surfaceToDock)
  {
  }
