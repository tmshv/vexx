#include "syimagetexture.h"
#include "sprocessmanager.h"
#include "QPainter"
#include "XGLRenderer.h"
#include "QGLContext"
#include "QThread"

S_IMPLEMENT_PROPERTY(SyImageTexture)

SPropertyInformation *SyImageTexture::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageTexture>("SyImageTexture");

  info->add(&SyImageTexture::texture, "texture");

  TransformProperty::InstanceInformation *transformInst = info->add(&SyImageTexture::transform, "transform");
  transformInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *widthInst = info->add(&SyImageTexture::width, "width");
  widthInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *heightInst = info->add(&SyImageTexture::height, "height");
  heightInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *imWidthInst = info->add(&SyImageTexture::imageWidth, "imageWidth");
  imWidthInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *imHeightInst = info->add(&SyImageTexture::imageHeight, "imageHeight");
  imHeightInst->setCompute(computeTransform);

  Vector2DProperty::InstanceInformation *imOffsetInst = info->add(&SyImageTexture::imageOffset, "imageOffset");
  imOffsetInst->setCompute(computeTransform);

  info->add(&SyImageTexture::input, "input");

  GCViewableTransformPointer::InstanceInformation *viewerInst = info->add(&SyImageTexture::viewer, "viewer");

  const SPropertyInstanceInformation *affects[] = {
    transformInst,
    widthInst,
    heightInst,
    imWidthInst,
    imHeightInst,
    imOffsetInst
  };
  viewerInst->setAffects(affects, X_ARRAY_COUNT(affects));

  return info;
  }

SyImageTexture::SyImageTexture() : _thread(0), _localWorker(0), _evaluator(0), _revision(0)
  {
  }

SyImageTexture::~SyImageTexture()
  {
  _thread->exit();
  while(_thread->isRunning())
    {
    QThread::yieldCurrentThread();
    }

  delete _localWorker;
  delete _evaluator;
  }

void SyImageTexture::computeTransform(const SPropertyInstanceInformation *, SyImageTexture *cont)
  {
  XVector3D lookAxis(0.0f, 0.0f, 1.0f);
  XVector3D xAxis(1.0f, 0.0f, 0.0f);
  XVector3D yAxis(0.0f, 1.0f, 0.0f);

  SPointerComputeLock cl(&cont->viewer);
  const GCViewableTransform* trans = cont->viewer.pointed();

  xuint32 vpW = trans->viewportWidth();
  xuint32 vpH = trans->viewportHeight();
  if(vpW > 0 && vpH > 0)
    {
    float dist = trans->transform().translation().dot(lookAxis);

    float x = trans->viewportX();
    float y = trans->viewportY();
    float w = trans->viewportWidth();
    float h = trans->viewportHeight();

    XVector3D tl = trans->worldSpaceAtDepthFromScreenSpace(x, y, dist);
    XVector3D tr = trans->worldSpaceAtDepthFromScreenSpace(x+w, y, dist);
    XVector3D bl = trans->worldSpaceAtDepthFromScreenSpace(x, y+h, dist);
    XVector3D br = trans->worldSpaceAtDepthFromScreenSpace(x+w, y+h, dist);

    TransformProperty::PODType imageTrans = TransformProperty::PODType::Identity();

    imageTrans.translation() = (tl + tr + bl + br) / 4.0f;

    cont->transform = imageTrans;
    XVector3D subA = tr - tl;
    XVector3D subB = br - bl;
    XVector3D subC = tl - bl;
    XVector3D subD = tr - br;

    float imW = ceilf(xMax(subA.dot(xAxis), subB.dot(xAxis)));
    float imH = ceilf(xMax(subC.dot(yAxis), subD.dot(yAxis)));

    cont->width = imW;
    cont->height = imH;

    float scaleX, scaleY;
    trans->approximatePixelSizeAtDistance(dist, scaleX, scaleY);

    XVector2D centre = imageTrans.translation().block<2, 1>(0, 0);
    cont->imageOffset = centre + XVector2D((-imW/2.0f)/scaleX, (-imH/2.0f)/scaleX);

    cont->imageWidth = trans->viewportWidth();
    cont->imageHeight = trans->viewportHeight();
    }
  }

void SyImageTexture::setup(QObject *inf, const char *c)
  {
  _thread = new QThread();
  _localWorker = new LocalWorker(this);
  _evaluator = new EvalObject(this);

  _evaluator->moveToThread(_thread);

  QObject::connect(_evaluator, SIGNAL(segmentFinished(int, int, QImage)), _localWorker, SLOT(loadIntoTexture(int, int, QImage)), Qt::QueuedConnection);
  QObject::connect(_localWorker, SIGNAL(updateSegment(int,int,int,int,int,int,int,int)), _evaluator, SLOT(updateSegment(int,int,int,int,int,int,int,int)), Qt::QueuedConnection);

  QObject::connect(_localWorker, SIGNAL(textureUpdated()), inf, c);
  }

void SyImageTexture::queueThreadedUpdate()
  {
  xAssert(_thread);
  xAssert(_evaluator)
  xAssert(_localWorker);

  if(!_thread->isRunning())
    {
    _thread->start();
    _thread->setPriority(QThread::IdlePriority);
    }

  static bool queueing = false;
  if(!queueing)
    {
      {
      QMutexLocker l(&_lock);
      _revision++;
      }
    queueing = true;

    xuint32 w = imageWidth();
    xuint32 h = imageHeight();

    static const xsize segSize = 64;
    xsize itW  = (w / segSize) + 1;
    xsize itH = (h / segSize) + 1;

    xsize s = itH * itW;
    XVectorI2D start = imageOffset().cast<xint32>();
    for(xsize i = 0; i < s; ++i)
      {
      xsize x = i % itW;
      xsize y = i / itW;
      _localWorker->emitUpdateSegment(_revision, x*segSize, y*segSize, start.x() + (x*segSize), start.y() + (y*segSize), 1, segSize, segSize);
      }
    queueing = false;
    }
  }

void SyImageTexture::postChildSet(SPropertyContainer *c, SProperty *prop)
  {
  SEntity::postChildSet(c, prop);
  SyImageTexture* tex = c->uncheckedCastTo<SyImageTexture>();

  if(SProcessManager::isMainThread() &&
    (prop == &tex->input ||
     prop == &tex->imageWidth ||
     prop == &tex->imageHeight ||
     prop == &tex->imageOffset))
    {
    tex->queueThreadedUpdate();
    }
  }

void SyImageTexture::LocalWorker::loadIntoTexture(int x, int y, QImage tex)
  {   
    {
    bool stateEnabled = _texture->database()->stateStorageEnabled();
    _texture->database()->setStateStorageEnabled(false);
    GCTexture::ComputeLock cL(&_texture->texture);
    QImage im = _texture->texture().texture();

    xuint32 w = _texture->imageWidth();
    xuint32 h = _texture->imageHeight();
    if(im.width() != w || im.height() != h)
      {
      im = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
      }

    QPainter p(&im);

    p.drawImage(x, y, tex);
    p.setPen(Qt::black);
    p.drawRect(x,y,1,1);

    cL.data()->load(im);
    if(!_texture->texture.isDirty())
      {
      _texture->texture.postSet();
      }

    _texture->database()->setStateStorageEnabled(stateEnabled);
    }

  emit textureUpdated();
  }

void SyImageTexture::EvalObject::updateSegment(int rev, int x, int y, int renderX, int renderY, int renderScale, int w, int h)
  {
    {
    QMutexLocker l(&_texture->_lock);
    if(rev != _texture->_revision)
      {
      return;
      }
    }

  QImage tmp;// = _texture->input.asQImage(XVectorI2D(renderX, renderY), 1, w, h);

  emit segmentFinished(x, y, tmp);
  QThread::yieldCurrentThread();
  }
