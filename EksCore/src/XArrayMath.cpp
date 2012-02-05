#include "XArrayMath"
#include "XAssert"
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "QRect"

XMathsEngine *g_engine = 0;
XMathsEngine *XMathsEngine::engine()
  {
  return g_engine;
  }

void XMathsEngine::setEngine(XMathsEngine *e)
  {
  g_engine = e;
  }

XMathsResult::XMathsResult(const XMathsOperation &o)
  {
  XMathsEngine::engine()->evaluateData(&o, o.userData(), &_dataType, &_data, &_dataWidth, &_dataHeight, &_dataChannels, &_transform);
  }

XMathsOperation::XMathsOperation() : _user(0), _nextUser(0), _inputA(0), _inputB(0), _userData(0)
  {
  xAssert(XMathsEngine::engine());
  _operation = NoOp;
  }

XMathsOperation::~XMathsOperation()
  {
  XMathsEngine::engine()->onCleanUp(this, &_userData);
  while(_user)
    {
    removeUser(_user);
    }
  }

void XMathsOperation::addUser(XMathsOperation *o) const
  {
  xAssert(o->_nextUser == 0);
  o->_nextUser = _user;
  _user = o;
  }

void XMathsOperation::removeUser(XMathsOperation *o) const
  {
  XMathsOperation *i = _user;
  XMathsOperation *prev = 0;
  while(i != o && i)
    {
    prev = i;
    i = i->_nextUser;
    }

  xAssert(i);
  if(prev)
    {
    prev->_nextUser = i->_nextUser;
    }
  i->_nextUser = 0;
  }

void XMathsOperation::setInput(const XMathsOperation **inp, const XMathsOperation *in)
  {
  if(in != (*inp))
    {
    if(*inp)
      {
      (*inp)->removeUser(this);
      }

    *inp = in;
    if(in)
      {
      in->addUser(this);
      }

    setInputDirty();
    }
  }

void XMathsOperation::setOperation(XMathsOperation::OperationType t)
  {
  if(_operation != t)
    {
    _operation = t;
    setOperationDirty();
    }
  }

void XMathsOperation::setValue(const XVector4D &v)
  {
  if(!_vectorData.isApprox(v))
    {
    _vectorData = v;
    setValueDirty();
    }
  }

void XMathsOperation::setValue(xuint32 v)
  {
  if(_integerData != v)
    {
    _integerData = v;
    setValueDirty();
    }
  }

void XMathsOperation::setInputDirty()
  {
  XMathsEngine::engine()->onInputDirty(this, &_userData);
  }

void XMathsOperation::setOperationDirty()
  {
  XMathsEngine::engine()->onOperationDirty(this, &_userData);
  }

void XMathsOperation::setValueDirty()
  {
  XMathsEngine::engine()->onValueDirty(this, &_userData);
  }

void XMathsOperation::load(DataType t, void* data, xsize dataWidth, xsize dataHeight, xsize dataChannels, const XMatrix3x3 &m)
  {
  setInput(&_inputA, 0);
  setInput(&_inputB, 0);

  setOperation(Load);
  _userData = XMathsEngine::engine()->loadData(t, data, dataWidth, dataHeight, dataChannels, m);
  }

void XMathsOperation::add(const XMathsOperation &a, const XMathsOperation &b)
  {
  setInput(&_inputA, &a);
  setInput(&_inputB, &b);

  setOperation(Add);
  }

void XMathsOperation::add(const XMathsOperation &a, const XVector4D &v)
  {
  setInput(&_inputA, &a);
  setInput(&_inputB, 0);

  setOperation(AddConst);
  setValue(v);
  }

void XMathsOperation::multiply(const XMathsOperation &a, const XMathsOperation &b)
  {
  setInput(&_inputA, &a);
  setInput(&_inputB, &b);

  setOperation(Multiply);
  }

void XMathsOperation::multiply(const XMathsOperation &a, const XVector4D &v)
  {
  setInput(&_inputA, &a);
  setInput(&_inputB, 0);

  setOperation(MultiplyConst);
  setValue(v);
  }

void XMathsOperation::convolve(const XMathsOperation &a, const XMathsOperation &b)
  {
  setInput(&_inputA, &a);
  setInput(&_inputB, &b);

  setOperation(Convolve);
  }

void XMathsOperation::shuffle(const XMathsOperation &a, xuint32 mask)
  {
  setInput(&_inputA, &a);
  setInput(&_inputB, 0);

  setOperation(Shuffle);
  setValue(mask);
  }

void XMathsOperation::splice(const XMathsOperation &a, const XMathsOperation &b, xuint32 mask)
  {
  setInput(&_inputA, &a);
  setInput(&_inputB, &b);

  setOperation(Splice);
  setValue(mask);
  }

template <typename T> struct Defs
  {
  typedef Eigen::Matrix <T, 4, 1> Vec;
  typedef Eigen::Array <Vec, Eigen::Dynamic, Eigen::Dynamic> Array;

  struct ImageRef
    {
    ImageRef(XMatrix3x3& tr, Array& im) : _transform(&tr), _image(&im) { }
    ImageRef() : _transform(0), _image(0) { }

    XMatrix3x3& transform() { return *_transform; }
    Array& image() { return *_image; }

    const XMatrix3x3& transform() const { return *_transform; }
    const Array& image() const { return *_image; }

    XVector3D xAxis() { return transform().col(0); }
    XVector3D yAxis() { return transform().col(1); }
    XVector2D translation() { return transform().col(2).block<2, 1>(0, 0); }
    void setTranslation(const XVector2D &in) { transform().col(2).block<2, 1>(0, 0) = in; }

    void expandToFit(const ImageRef &im, QRectF& rect)
      {
      XMatrix3x3 m = im.transform() * transform().inverse();
      XVector3D a = m * XVector3D(0, 0, 1);
      XVector3D b = m * XVector3D(0, im.image().cols(), 1);
      XVector3D c = m * XVector3D(im.image().rows(), 0, 1);
      XVector3D d = m * XVector3D(im.image().rows(), im.image().cols(), 1);

      rect = rect.unite(QRectF(a.x(), a.y(), 1, 1));
      rect = rect.unite(QRectF(b.x(), b.y()-1, 1, 1));
      rect = rect.unite(QRectF(c.x()-1, c.y(), 1, 1));
      rect = rect.unite(QRectF(d.x()-1, d.y()-1, 1, 1));
      }

    void setSize(const QRectF& rect)
      {
      setTranslation(translation() + XVector2D(rect.x(), rect.y()));
      image().resize(rect.width()+1.0f, rect.height()+1.0f);
      }

    Vec sampleFrom(const XMatrix3x3 &mapping, const XVector3D &pt) const
      {
      XVector3D mappedPt = mapping * pt;

      if(mappedPt.x() < 0.0f || mappedPt.y() < 0.0f ||
         mappedPt.x() >= image().rows() || mappedPt.y() >= image().cols())
        {
        return Vec::Zero();
        }

      return image()(mappedPt.x(), mappedPt.y());
      }

    Vec sampleFrom(const XVector3D &pt) const
      {
      if(pt.x() < 0.0f || pt.y() < 0.0f ||
         pt.x() >= image().rows() || pt.y() >= image().cols())
        {
        return Vec::Zero();
        }

      return image()(pt.x(), pt.y());
      }

  private:
    XMatrix3x3 *_transform;
    Array *_image;
    };
  };

struct ReferenceMathsEngineResult
  {
  XMathsOperation::DataType _type;
  xuint8 _channels;
  XMatrix3x3 _transform;

  Defs<float>::Array _floats;
  Defs<xuint32>::Array _ints;
  };

template <typename T> struct Utils
  {
  typedef typename Defs<T>::Array Array;
  typedef typename Defs<T>::Vec Vec;
  typedef typename Defs<T>::ImageRef ImageRef;
  typedef void (*InitFunction)(const XMathsOperation* o, ImageRef &arr, const ImageRef *a, const ImageRef *b);
  typedef void (*MathsFunction)(const XVector3D &pt, const XMathsOperation* o, const XMatrix3x3& mat, Vec &arr, const XMatrix3x3 &, const ImageRef *a, const XMatrix3x3 &, const ImageRef *b);

  static void unite(const XMathsOperation* o, ImageRef &arr, const ImageRef *a, const ImageRef *b)
    {
    xAssert(a);
    xAssert(b);

    arr.transform() = a->transform();

    QRectF r;
    arr.expandToFit(*a, r);
    arr.expandToFit(*b, r);

    arr.setSize(r);
    }

  static void takeA(const XMathsOperation*, ImageRef &arr, const ImageRef *a, const ImageRef *b)
    {
    xAssert(a);
    arr.transform() = a->transform();
    arr.image().resize(a->image().rows(), a->image().cols());
  }

  static void add(const XVector3D &pt, const XMathsOperation* o, const XMatrix3x3& mat, Vec &arr, const XMatrix3x3 &mapA, const ImageRef *a, const XMatrix3x3 &mapB, const ImageRef *b)
  {
    xAssert(a);
    xAssert(b);

    arr = a->sampleFrom(mapA, pt) + b->sampleFrom(mapB, pt);
  }

  static void convolve(const XVector3D &pt, const XMathsOperation* o, const XMatrix3x3& mat, Vec &arr, const XMatrix3x3 &mapA, const ImageRef *a, const XMatrix3x3 &, const ImageRef *b)
  {
    xAssert(a);
    xAssert(b);

    xAssert(mapA.isIdentity());

    arr = Vec::Zero();
    for(xsize y = 0, h = b->image().cols(); y < h; ++y)
      {
      for(xsize x = 0, w = b->image().rows(); x < w; ++x)
        {
        XVector3D samplePt = pt + b->transform() * XVector3D(x, y, 1.0f);

        Vec::Scalar factor = b->image()(x, y)(0);
        Vec comp = a->sampleFrom(samplePt);

        arr += comp * factor;
        }
      }
  }

  static void doOperation(const XMathsOperation* o, ImageRef &arr, const ImageRef *a, const ImageRef *b)
    {
    static const InitFunction iFns[] =
      {
      0,     // NoOp
      0,     // Load
      unite, // Add
      takeA, // AddConst
      unite, // Multiply
      takeA, // MultiplyConst
      takeA, // Convolve
      takeA, // Shuffle
      unite  // Splice
      };

    xAssert(o->operation() < sizeof(iFns)/sizeof(iFns[0]));
    xAssert(iFns[o->operation()]);
    iFns[o->operation()](o, arr, a, b);

    XMatrix3x3 mapAToBase;
    XMatrix3x3 mapBToBase;

    if(a)
      {
      mapAToBase = a->transform().inverse() * arr.transform();
      }
    if(b)
      {
      mapBToBase = b->transform().inverse() * arr.transform();
      }

    static const MathsFunction fFns[] =
      {
      0,        // NoOp
      0,        // Load
      add,      // Add
      0,        // AddConst
      0,        // Multiply
      0,        // MultiplyConst
      convolve, // Convolve
      0,        // Shuffle
      0         // Splice
      };

    xAssert(o->operation() < sizeof(fFns)/sizeof(fFns[0]));
    MathsFunction fn = fFns[o->operation()];
    xAssert(fn);

    for(int y = 0; y < arr.image().cols(); ++y)
      {
      for(int x = 0; x < arr.image().rows(); ++x)
        {
        XVector3D pt(x, y, 1.0);

        Vec& v = arr.image()(x, y);
        fn(pt, o, arr.transform(), v, mapAToBase, a, mapBToBase, b);
        }
      }
    }
  };

void XReferenceMathsEngine::evaluateData(const XMathsOperation *, const void *userData, XMathsOperation::DataType *type, const void **data, xsize *dataWidth, xsize *dataHeight, xsize *dataChannels, XMatrix3x3 *m)
  {
  const ReferenceMathsEngineResult *result = (const ReferenceMathsEngineResult *)userData;

  *type = result->_type;
  *dataChannels = result->_channels;
  *m = result->_transform;

  if(result->_type == XMathsOperation::Float)
    {
    *data = result->_floats.data();
    *dataWidth = result->_floats.rows();
    *dataHeight = result->_floats.cols();
    }
  else if(result->_type == XMathsOperation::Byte)
    {
    *data = result->_ints.data();
    *dataWidth = result->_ints.rows();
    *dataHeight = result->_ints.cols();
    }
  else
    {
    xAssertFail();
    }
  }

void *XReferenceMathsEngine::loadData(XMathsOperation::DataType type, void* data, xsize width, xsize height, xuint8 channels, const XMatrix3x3 &m)
  {
  ReferenceMathsEngineResult *ret = new ReferenceMathsEngineResult;
  ret->_type = type;
  ret->_channels = channels;
  ret->_transform = m;

  xAssert(channels <= 4);


  if(type == XMathsOperation::Float)
    {
    float *dataPtr = (float*)data;
    ret->_floats.resize(width, height);

    for(int y = 0; y < ret->_floats.cols(); ++y)
      {
      for(int x = 0; x < ret->_floats.rows(); ++x)
        {
        XVector4D& v = ret->_floats(x, y);
        memcpy(v.data(), dataPtr, sizeof(float)*channels);
        dataPtr += channels;
        }
      }
    }
  else if(type == XMathsOperation::Byte)
    {
    xuint8 *dataPtr = (xuint8*)data;
    ret->_ints.resize(width, height);

    for(int y = 0; y < ret->_ints.cols(); ++y)
      {
      for(int x = 0; x < ret->_ints.rows(); ++x)
        {
        Defs<xuint32>::Vec& v = ret->_ints(x, y);
        for(int w = 0; w < channels; ++w)
          {
          v(w) = dataPtr[w];
          }
        dataPtr += channels;
        }
      }
    }
  else
    {
    xAssertFail();
    }

  return ret;
  }

void XReferenceMathsEngine::onOperationDirty(const XMathsOperation *o, void **userData)
  {
  ReferenceMathsEngineResult *&res = *(ReferenceMathsEngineResult**)userData;
  if(!res)
    {
    res = new ReferenceMathsEngineResult;
    res->_transform = XMatrix3x3::Identity();
    res->_type = XMathsOperation::None;
    res->_channels = 0;
    }

  if(o->operation() == XMathsOperation::NoOp || o->operation() == XMathsOperation::Load)
  {
    xAssert(*userData);
    return;
    }

  static const xuint8 expectedInputs[] =
  {
    0, // NoOp
    0, // Load
    2, // Add
    1, // AddConst
    2, // Multiply
    1, // MultiplyConst
    2, // Convolve
    1, // Shuffle
    2  // Splice
  };

  xuint8 expInputs = expectedInputs[o->operation()];
  ReferenceMathsEngineResult *a = (ReferenceMathsEngineResult*)(o->inputA() ? o->inputA()->userData() : 0);
  if(expInputs >= 1 && !a)
    {
    xAssertFail();
    return;
    }

  ReferenceMathsEngineResult *b = (ReferenceMathsEngineResult*)(o->inputB() ? o->inputB()->userData() : 0);
  if(expInputs >= 2 && !b)
    {
    xAssertFail();
    return;
    }

  xAssert(a);

  res->_channels = 4;//a->_channels;
  res->_type = a->_type;
  if(a->_type == XMathsOperation::Float)
    {
    Utils<float>::ImageRef aIm;
    if(a)
      {
      aIm = Utils<float>::ImageRef(a->_transform, a->_floats);
      }

    Utils<float>::ImageRef bIm;
    if(b)
      {
      bIm = Utils<float>::ImageRef(b->_transform, b->_floats);
      }

    Utils<float>::ImageRef im(res->_transform, res->_floats);
    Utils<float>::doOperation(o, im, &aIm, &bIm);
    }
  else if(a->_type == XMathsOperation::Byte)
    {
    Utils<xuint32>::ImageRef aIm;
    if(a)
      {
      aIm = Utils<xuint32>::ImageRef(a->_transform, a->_ints);
      }

    Utils<xuint32>::ImageRef bIm;
    if(b)
      {
      bIm = Utils<xuint32>::ImageRef(b->_transform, b->_ints);
      }

    Utils<xuint32>::ImageRef im(res->_transform, res->_ints);
    Utils<xuint32>::doOperation(o, im, &aIm, &bIm);
    }
  }

void XReferenceMathsEngine::onValueDirty(const XMathsOperation *o, void **userData)
  {
  onOperationDirty(o, userData);
  }

void XReferenceMathsEngine::onInputDirty(const XMathsOperation *o, void **userData)
  {
  onOperationDirty(o, userData);
  }

void XReferenceMathsEngine::onCleanUp(const XMathsOperation *, void **userData)
  {
  ReferenceMathsEngineResult *&res = *(ReferenceMathsEngineResult**)userData;
  delete res;
  }
