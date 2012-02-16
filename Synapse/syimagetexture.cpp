#include "syimagetexture.h"
#include "QThread"
#include "QPainter"

S_IMPLEMENT_PROPERTY(SyImageTexture)

SPropertyInformation *SyImageTexture::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageTexture>("SyImageTexture");

  GCTexture::InstanceInformation *textureInst = info->add(&SyImageTexture::texture, "texture");
  textureInst->setCompute(computeTexture);

  TransformProperty::InstanceInformation *transformInst = info->add(&SyImageTexture::transform, "transform");
  transformInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *widthInst = info->add(&SyImageTexture::width, "width");
  widthInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *heightInst = info->add(&SyImageTexture::height, "height");
  heightInst->setCompute(computeTransform);

  UnsignedIntProperty::InstanceInformation *imWidthInst = info->add(&SyImageTexture::imageWidth, "imageWidth");
  imWidthInst->setCompute(computeTransform);
  imWidthInst->setAffects(textureInst);

  UnsignedIntProperty::InstanceInformation *imHeightInst = info->add(&SyImageTexture::imageHeight, "imageHeight");
  imHeightInst->setCompute(computeTransform);
  imHeightInst->setAffects(textureInst);

  Vector2DProperty::InstanceInformation *imOffsetInst = info->add(&SyImageTexture::imageOffset, "imageOffset");
  imOffsetInst->setCompute(computeTransform);
  imOffsetInst->setAffects(textureInst);

  SyImageInput::InstanceInformation *inputInst = info->add(&SyImageTexture::input, "input");
  inputInst->setAffects(textureInst);

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

class SyImageTexture::EvalThread : public QThread
  {
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

  virtual void run()
    {
    bool stateEnabled = _texture->database()->stateStorageEnabled();
    _texture->database()->setStateStorageEnabled(false);

    qDebug() << "Run Thread";
    do
    {
      qDebug() << "Start job";
      QImage im;
      xuint32 w;
      xuint32 h;
      
      im = _texture->texture().texture();
        {
        QMutexLocker l(&_lock);
        _do = false;

        w = _texture->imageWidth();
        h = _texture->imageHeight();

        if(im.width() != w || im.height() != h)
          {
          im = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
          }
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
          GCTexture::ComputeLock cL(&_texture->texture);

          //tmp = _texture.input.asQImage(start + XVectorI2D(segSize*x, segSize*y), segSize, segSize);
          tmp.fill(QColor(rand()%255, rand()%255, rand()%255));

          QPainter p(&im);

          p.drawImage(x*segSize, y*segSize, tmp);

          cL.data()->load(im);
          _texture->postSet();
          QThread::msleep(100);
          }
        }
      qDebug() << "job complete" << _do;
      } while(_do == true);

    _texture->database()->setStateStorageEnabled(stateEnabled);
    }

private:
  QMutex _lock;
  bool _do;
  };

void SyImageTexture::queueThreadedUpdate()
  {
  if(!_loadThread)
    {
    _loadThread = new EvalThread(this);
    }

  _loadThread->reset();

  if(!_loadThread->isRunning())
    {
    _loadThread->start();
    }
  }

void SyImageTexture::computeTexture(const SPropertyInstanceInformation *, SyImageTexture *cont)
  {
  qDebug() << "Eval Texture";
  cont->queueThreadedUpdate();
  }
