#ifndef SYIMAGETEXTURE_H
#define SYIMAGETEXTURE_H

#include "sentity.h"
#include "syimageinput.h"
#include "GCBaseProperties.h"
#include "3D/GCCamera.h"
#include "3D/GCTexture.h"
#include "QObject"

class SyImageTexture : public SEntity
  {
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

  void setup(QObject *inf, const char *);

  static void postChildSet(SPropertyContainer *, SProperty *);

private:
  void queueThreadedUpdate();

  static void computeTransform(const SPropertyInstanceInformation *, SyImageTexture *cont);

  QMutex _lock;
  int _revision;

  class EvalObject;
  friend class EvalObject;
  class LocalWorker;
  QThread *_thread;
  EvalObject *_evaluator;
  LocalWorker *_localWorker;
  };

class SyImageTexture::LocalWorker : public QObject
  {
  Q_OBJECT
public:
  LocalWorker(SyImageTexture* t) : _texture(t) { }
  
  void emitUpdateSegment(int rev, int x, int y, int renderX, int renderY, int renderScale, int w, int h)
    {
    emit updateSegment(rev, x, y, renderX, renderY, renderScale, w, h);
    }

signals:
  void textureUpdated();
  void updateSegment(int rev, int x, int y, int renderX, int renderY, int renderScale, int w, int h);

public slots:
  void loadIntoTexture(int x, int y, QImage tex);

private:
  SyImageTexture *_texture;
  };

class SyImageTexture::EvalObject : public QObject
  {
  Q_OBJECT

XProperties:
  XProperty(SyImageTexture *, texture, setTexture);

public:
  EvalObject(SyImageTexture *i) : _texture(i)
    {
    }

public slots:
  void updateSegment(int rev, int x, int y, int renderX, int renderY, int renderScale, int w, int h);

signals:
  void segmentFinished(int x, int y, QImage im);
  };

#endif // SYIMAGETEXTURE_H
