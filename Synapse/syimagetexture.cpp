#include "syimagetexture.h"
#include "sprocessmanager.h"
#include "QPainter"
#include "XGLRenderer.h"
#include "QGLContext"

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

SyImageTexture::SyImageTexture() : _loadThread(0)
  {
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
  _localWorker = new LocalWorker(this);
  _loadThread = new EvalThread(this);
  QObject::connect(_loadThread, SIGNAL(segmentFinished(int, int, QImage)), _localWorker, SLOT(loadIntoTexture(int, int, QImage)), Qt::QueuedConnection);
  QObject::connect(_localWorker, SIGNAL(textureUpdated()), inf, c);
  }

void SyImageTexture::queueThreadedUpdate()
  {
  xAssert(_loadThread && _localWorker);

  /*_loadThread->reset();

  if(!_loadThread->isRunning())
    {
    _loadThread->start();
    }*/
  
  static bool i = 0;
  if(!i)
    {
    i = 1;

    for(int i = 0; i < 100; ++i)
      {
      _localWorker->loadIntoTexture(0,0,QImage());
      }
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
    //bool stateEnabled = _texture->database()->stateStorageEnabled();
    //_texture->database()->setStateStorageEnabled(false);
    GCTexture::ComputeLock cL(&_texture->texture);
    QImage im = _texture->texture().texture();

    xuint32 w = 1;//_texture->imageWidth();
    xuint32 h = 1;//_texture->imageHeight();
    if(im.width() != w || im.height() != h)
      {
      im = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
      }

    /*QPainter p(&im);

    p.drawImage(x, y, tex);
    p.drawText(x, y, QString::number(x + y));*/

    /*static XGLRenderer *r = 0;
    static XShader *s = 0;
    if(!r)
      {
      r = new XGLRenderer;
      r->setContext(new QGLContext(QGLFormat::defaultFormat()));
      s = new XShader;
      }

    s->getVariable("abs")->setValue(*cL.data());

    cL.data()->load(im);
    cL.data()->prepareInternal(r);
    s->prepareInternal(r);*/

    cL.data()->load(im);
    if(!_texture->texture.isDirty())
      {
      _texture->texture.postSet();
      }

    //_texture->database()->setStateStorageEnabled(stateEnabled);
    }

  emit textureUpdated();
  }

void SyImageTexture::EvalThread::run()
{
  qDebug() << "Run Thread";
  do
  {
    qDebug() << "Start job";
    xuint32 w;
    xuint32 h;

    {
      QMutexLocker l(&_lock);
      _do = false;

      w = _texture->imageWidth();
      h = _texture->imageHeight();
    }

    static const xsize segSize = 64;
    xsize itW  = (w / segSize) + 1;
    xsize itH = (h / segSize) + 1;


    qDebug() << "Dims:" << w << h;
    qDebug() << "Segs:" << itW << itH;

    QImage tmp(segSize, segSize, QImage::Format_ARGB32_Premultiplied);
    XVectorI2D start = _texture->imageOffset().cast<xint32>();
    for(xsize y = 0; y < itH; ++y)
    {
      for(xsize x = 0; x < itW; ++x)
      {
        //tmp = _texture.input.asQImage(start + XVectorI2D(segSize*x, segSize*y), segSize, segSize);
        tmp.fill(QColor(rand()%255, rand()%255, rand()%255));

        emit segmentFinished(x*segSize, y*segSize, tmp);
        //QThread::wait(100);
        yieldCurrentThread();
      }
    }
    qDebug() << "job complete" << _do;
  } while(_do == true);
}
