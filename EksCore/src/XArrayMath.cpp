#include "XArrayMath"
#include "XAssert"
#include "Eigen/Core"
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

XMathsResult::XMathsResult(const XMathsOperation &)
  {
  }

XMathsOperation::DataType XMathsResult::dataType() const
  {
  return XMathsOperation::Float;
  }

xuint8 *XMathsResult::data() const
  {
  return 0;
  }

xsize XMathsResult::dataWidth() const
  {
  return 0;
  }

xsize XMathsResult::dataHeight() const
  {
  return 0;
  }

XMathsOperation::XMathsOperation() : _user(0), _nextUser(0), _inputA(0), _inputB(0), _userData(0)
  {
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
  prev->_nextUser = i->_nextUser;
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

void XMathsOperation::load(DataType t, void* data, xsize dataWidth, xsize dataHeight, xsize dataChannels)
  {
  setInput(&_inputA, 0);
  setInput(&_inputB, 0);

  setOperation(Load);
  _userData = XMathsEngine::engine()->loadData(t, data, dataWidth, dataHeight, dataChannels);
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
  };

struct ReferenceMathsEngineResult
  {
  XMathsOperation::DataType _type;
  xuint8 _channels;
  XMatrix3x3 _transform;

  Defs<float>::Array _floats;
  Defs<xuint32>::Array _ints;

  QRect rect() const
    {
    }
  };

template <typename T> struct Utils
  {
  typedef typename Defs<T>::Array Array;
  typedef typename Defs<T>::Vec Vec;
  typedef void (*InitFunction)(const XMathsOperation* o, XMatrix3x3& mat, Array &arr, const ReferenceMathsEngineResult *a, const ReferenceMathsEngineResult *b);
  typedef void (*MathsFunction)(const XVector3D &pt, const XMathsOperation* o, const XMatrix3x3& mat, Vec &arr, const ReferenceMathsEngineResult *a, const ReferenceMathsEngineResult *b);

  static void unite(const XMathsOperation* o, XMatrix3x3& mat, Array &arr, const ReferenceMathsEngineResult *a, const ReferenceMathsEngineResult *b)
    {
    }

  static void takeA(const XMathsOperation*, XMatrix3x3& mat, Array &arr, const ReferenceMathsEngineResult *a, const ReferenceMathsEngineResult *b)
    {
    mat = a->_transform;
    arr.resize(a->rows(), a->cols());
    }

  static void add(const XVector3D &pt, const XMathsOperation* o, const XMatrix3x3& mat, Vec &arr, const ReferenceMathsEngineResult *a, const ReferenceMathsEngineResult *b)
    {
    }

  static void doOperation(const XMathsOperation* o, XMatrix3x3& mat, Array &arr, const ReferenceMathsEngineResult *a, const ReferenceMathsEngineResult *b)
    {
    static const InitFunction iFns[] =
      {
      0,     // Load
      unite, // Add
      takeA, // AddConst
      unite, // Multiply
      takeA, // MultiplyConst
      takeA, // Convolve
      takeA, // Shuffle
      unite  // Splice
      };

    xAssert(iFns[o->operation()]);
    iFns[o->operation()](o, mat, arr, a, b);

    static const MathsFunction fFns[] =
      {
      0,    // Load
      add,  // Add
      0,    // AddConst
      0,    // Multiply
      0,    // MultiplyConst
      0,    // Convolve
      0,    // Shuffle
      0     // Splice
      };

    MathsFunction fn = fFns[o->operation()];
    xAssert(fn);

    for(int y = 0; y < arr.cols(); ++y)
      {
      for(int x = 0; x < arr.rows(); ++x)
        {
        XVector3D pt(x, y, 1.0);

        Vec& v = arr(x, y);
        fn(pt, o, mat, v, a, b);
        }
      }
    }
  };

void *XReferenceMathsEngine::loadData(XMathsOperation::DataType type, void* data, xsize width, xsize height, xuint8 channels)
  {
  ReferenceMathsEngineResult *ret = new ReferenceMathsEngineResult;
  ret->_type = type;
  ret->_channels = channels;

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
  const ReferenceMathsEngineResult *a = (ReferenceMathsEngineResult*)(o->inputA() ? o->inputA()->userData() : 0);
  if(expInputs >= 1 && !a)
    {
    xAssertFail();
    return;
    }

  const ReferenceMathsEngineResult *b = (ReferenceMathsEngineResult*)(o->inputB() ? o->inputB()->userData() : 0);
  if(expInputs >= 2 && !b)
    {
    xAssertFail();
    return;
    }

  xAssert(a);

  res->_channels = a->_channels;
  res->_type = a->_type;
  if(a->_type == XMathsOperation::Float)
    {
    Utils<float>::doOperation(o, res->_transform, res->_floats, a, b);
    }
  else if(a->_type == XMathsOperation::Byte)
    {
    Utils<xuint32>::doOperation(o, res->_transform, res->_ints, a, b);
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
