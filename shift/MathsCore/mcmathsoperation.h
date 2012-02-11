#ifndef MCMATHSOPERATION_H
#define MCMATHSOPERATION_H

#include "mcglobal.h"
#include "sbaseproperties.h"
#include "XArrayMath"

class MATHSCORE_EXPORT MCMathsOperation : public SProperty
  {
  S_PROPERTY(MCMathsOperation, SProperty, 0)
XProperties:
  XPropertyMember(XMathsOperation, value);

public:
  class ComputeLock
    {
  public:
    ComputeLock(MCMathsOperation *ptr) : _ptr(ptr)
      {
      xAssert(ptr);
      _data = &_ptr->_value;
      }
    ~ComputeLock()
      {
      _ptr->handler()->doChange<ComputeChange>(_ptr);
      }

    XMathsOperation* data()
      {
      return _data;
      }

  private:
    MCMathsOperation *_ptr;
    XMathsOperation* _data;
    };

  const XMathsOperation &operator()() const
    {
    preGet();
    return _value;
    }

  const XMathsOperation &value() const
    {
    preGet();
    return _value;
    }

  static void saveProperty(const SProperty *, SSaver &)
    {
    xAssertFail();
    }

  static SProperty *loadProperty(SPropertyContainer *, SLoader &)
    {
    xAssertFail();
    return 0;
    }

  static bool shouldSavePropertyValue(const SProperty *)
    {
    return false;
    }

  bool saveResultToFile(QString filename);

private:
  class ComputeChange : public SProperty::DataChange
    {
    S_CHANGE(ComputeChange, SProperty::DataChange, 200);

  public:
    ComputeChange(MCMathsOperation *prop)
      : SProperty::DataChange(prop)
      {
      xAssert(!prop->database()->stateStorageEnabled());
      }

  private:
    bool apply()
      {
      return true;
      }
    bool unApply()
      {
      xAssertFail();
      return true;
      }
    bool inform()
      {
      if(property()->entity())
        {
        property()->entity()->informDirtyObservers(property());
        }
      return true;
      }
    };

  friend class ComputeLock;
  };

#endif // MCMATHSOPERATION_H
