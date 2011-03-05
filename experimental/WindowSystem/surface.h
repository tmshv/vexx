#ifndef SURFACE_H
#define SURFACE_H

#include "QWidget"

class Surface : public QWidget
  {
public:
  enum HitMode
    {
    HitTop,
    HitLeft,
    HitRight,
    HitBottom,
    HitStack,
    HitClose
    };
  enum DockMode
    {
    DockTop,
    DockLeft,
    DockRight,
    DockBottom
    };

  Surface(QWidget *);
  ~Surface();

  Surface *getMainWidget();
  const Surface *getMainWidget() const;
  bool isMainSurface() const;

  void dock(DockMode, Surface *surfaceToDock);

  virtual quint32 hitTest(QPoint pt, Surface *) const;
  virtual void apply(quint32 mode, Surface *);

protected:
  virtual void moveEvent(QMoveEvent *);
#ifdef Q_OS_WIN
  virtual bool winEvent(MSG *message, long *result);
#endif

private:
  void finaliseMove();
  static Surface *_mainSurface;
  static QList<Surface*> _surfaces;
  quint32 _previousHitMode;
  Surface *_previousHitSurface;

  QWidget *_widget;
  };

#endif // SURFACE_H
