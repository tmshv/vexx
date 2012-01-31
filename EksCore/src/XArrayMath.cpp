#include "XArrayMath"
#include "XAssert"

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

struct ReferenceMathsEngineResult
  {
  XMathsOperation::DataType _type;
  xsize _width;
  xsize _height;
  xuint8 _channels;
  union
    {
    float *_floats;
    xuint8 *_bytes;
    };

  typedef void (*FloatMathsFunction)(XVector4D&, const XMathsOperation *o);
  };

void *XReferenceMathsEngine::loadData(XMathsOperation::DataType type, void* data, xsize width, xsize height, xuint8 channels)
  {
  ReferenceMathsEngineResult *ret = new ReferenceMathsEngineResult;
  ret->_type = type;
  ret->_width = width;
  ret->_height = height;
  ret->_channels = channels;
  if(type == XMathsOperation::Float)
    {
    ret->_floats = new float [width * height * channels];
    memcpy(ret->_floats, data, width*height*sizeof(float)*channels);
    }
  else if(type == XMathsOperation::Byte)
    {
    ret->_bytes = new xuint8 [width * height * channels];
    memcpy(ret->_bytes, data, width*height*sizeof(xuint8)*channels);
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
    res->_type = XMathsOperation::None;
    res->_width = 0;
    res->_height = 0;
    res->_channels = 0;
    }
  else
    {
    if(res->_type == XMathsOperation::Float)
      {
      delete [] res->_floats;
      }
    else if(res->_type == XMathsOperation::Byte)
      {
      delete [] res->_bytes;
      }
    }

  if(o->operation() == XMathsOperation::NoOp || o->operation() == XMathsOperation::Load)
  {
    xAssert(*userData);
    return;
    }

  const ReferenceMathsEngineResult *a = (ReferenceMathsEngineResult*)(o->inputA() ? o->inputA()->userData() : 0);
  const ReferenceMathsEngineResult *b = (ReferenceMathsEngineResult*)(o->inputB() ? o->inputB()->userData() : 0);

  xsize aLen = a ? a->_width * a->_height : 0;
  xsize bLen = b ? b->_width * b->_height : 0;
  xsize minLen = xMin(aLen, bLen);

  xsize aLenC = a ? aLen * a->_channels : 0;
  xsize bLenC = b ? bLen * b->_channels : 0;
  xsize minLenC = xMin(aLenC, bLenC);

  xAssert(!a || !b || a->_type == b->_type);
  XMathsOperation::DataType type = a ? a->_type : XMathsOperation::Float;

  if(type == XMathsOperation::Float)
    {
    res->_floats = new float[minLenC];
    }
  else if(type == XMathsOperation::Byte)
    {
    res->_bytes = new xuint8[minLenC];
    }

  if(type == XMathsOperation::Float)
    {
    static const ReferenceMathsEngineResult::FloatMathsFunction fFns[] =
      {
      Load,
      Add,
      AddConst,
      Multiply,
      MultiplyConst,
      Convolve,
      Shuffle,
      Splice
      };

    ReferenceMathsEngineResult::FloatMathsFunction fn = fFns[o->operation()];

    for(xsize y = 0; y < a->_height; ++y)
      {
      for(xsize x = 0; x < a->_width; ++x)
        {
        fn();
        }
      }
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
  if(res)
    {
    if(res->_type == XMathsOperation::Float)
      {
      delete [] res->_floats;
      }
    else if(res->_type == XMathsOperation::Float)
      {
      delete [] res->_bytes;
      }
    }
  delete res;
  }
