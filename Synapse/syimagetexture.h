#ifndef SYIMAGETEXTURE_H
#define SYIMAGETEXTURE_H

#include "sentity.h"
#include "syimageinput.h"
#include "GCBaseProperties.h"
#include "3D/GCCamera.h"
#include "3D/GCTexture.h"
#include "QThread"

class SyImageTexture : public QObject, public SEntity
  {
  Q_OBJECT
  S_ENTITY(SyImageTexture, SEntity, 0);

public:
  SyImageTexture();

  SyImageInput input;
  GCViewableTransformPointer viewer;

  GCTexture texture;
  TransformProperty transform;
  UnsignedIntProperty width;
  UnsignedIntProperty height;

  Vector2DProperty imageOffset;
  UnsignedIntProperty imageWidth;
  UnsignedIntProperty imageHeight;

private slots:
  void loadIntoTexture(int x, int y, QImage tex);

private:
  void queueThreadedUpdate();

  static void computeTransform(const SPropertyInstanceInformation *, SyImageTexture *cont);
  static void computeTexture(const SPropertyInstanceInformation *info, SyImageTexture *cont);

  class EvalThread;
  EvalThread* _loadThread;
  };

class SyImageTexture::EvalThread : public QThread
{
  Q_OBJECT
XProperties:
  XProperty(SyImageTexture *, texture, setTexture);

public:
  EvalThread(SyImageTexture *i) : _texture(i), _do(false)
  {
  }

  void reset()
  {
    QMutexLocker l(&_lock);
    _do = true;
  }

  virtual void run();

signals:
  void segmentFinished(int x, int y, QImage im);

private:
  QMutex _lock;
  bool _do;
};

#endif // SYIMAGETEXTURE_H
