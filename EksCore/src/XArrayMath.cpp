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

XMathsOperation::XMathsOperation() : _user(0), _nextUser(0), _inputA(0), _inputB(0)
  {
  }

XMathsOperation::~XMathsOperation()
  {
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

void XMathsOperation::setInput(XMathsOperation **inp, const XMathsOperation *in)
  {
  if(in != (*inp))
    {
    if(*inp)
      {
      (*inp)->removeUser(this);
      }

    if(in)
      {
      in->addUser(this);
      }

    setOperationDirty();
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

void XMathsOperation::setValue(void *v)
  {
  if(_dataPtr != v)
    {
    _dataPtr = v;
    setValueDirty();
    }
  }

void XMathsOperation::setValue(const XVector4D &v)
  {
  if(!_dataVec.isApprox(v))
    {
    _dataVec = v;
    setValueDirty();
    }
  }

void XMathsOperation::setValue(xuint32 v)
  {
  if(_dataInt != v)
    {
    _dataInt = v;
    setValueDirty();
    }
  }

void XMathsOperation::setOperationDirty()
  {
  }


void XMathsOperation::setValueDirty()
  {
  }

void XMathsOperation::load(DataType t, xuint8* data, xsize dataWidth, xsize dataHeight, xsize dataChannels)
  {
  setInput(&_inputA, 0);
  setInput(&_inputB, 0);

  setOperation(Load);
  setValue(XMathsEngine::engine()->loadData(t, data, dataWidth, dataHeight, dataChannels));
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

