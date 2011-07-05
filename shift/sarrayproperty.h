#ifndef SARRAYPROPERTY_H
#define SARRAYPROPERTY_H

#include "spropertycontainer.h"
#include "sbaseproperties.h"
#include "XList"
#include "sloader.h"
#include "Eigen/Core"

// reimplement stream for QTextStream to allow it to work with template classes

template <typename T, typename U> class SArrayProperty : public SProperty
  {
public:
  typedef T ElementType;
  typedef Eigen::Array <T, Eigen::Dynamic, Eigen::Dynamic> EigenArray;

  const EigenArray &data() const { preGet(); return mData; }

  void add(const SArrayProperty <T, U> *in)
    {
    EigenArray result = mData + in->data();
    doChange(result);
    }

  void add(const SArrayProperty <T, U> *inA, const SArrayProperty <T, U> *inB)
    {
    doChange(inA->data() + inB->data());
    }

  void resize(xsize width, xsize height)
    {
    EigenArray result = mData;
    result.resize(height, width);

    doChange(result);
    }

  void resize(xsize size)
    {
    EigenArray result = mData;
    result.resize(1, width);

    doChange(result);
    }

  xsize size() const
    {
    preGet();
    return mData.cols();
    }

  xsize width() const
    {
    preGet();
    return mData.cols();
    }

  xsize height() const
    {
    preGet();
    return mData.rows();
    }

  const T *get() const
    {
    preGet();
    return mData.data();
    }

  void set(xsize width, xsize height, const XVector<T> &val)
    {
    EigenArray result = mData;
    result.resize(height, width);

    memcpy(result.data(), &val.front(), sizeof(T)*width*height);

    doChange(result);
    }

  void setData(const EigenArray &result)
    {
    doChange(result);
    }

  void setIndex(xsize x, xsize y, const T &val)
    {
    EigenArray result = mData;
    result(y, x) = val;

    doChange(result);
    }

  // called by parent
  static void saveProperty( const SProperty* p_in, SSaver &l); // Mode = Binary / ASCII
  static SProperty *loadProperty( SPropertyContainer* p_in, SLoader&); // Mode = Binary / ASCII
  static void AssignFunction(const SProperty *from, SProperty *to)
    {
    const U *f = from->castTo<U>();
    U *t = to->castTo<U>();

    xAssert(f && t);
    if(f && t)
      {
      t->mData = f->mData;
      }
    }

private:
  class Change : public SProperty::DataChange
    {
    S_CHANGE( Change, SChange, Type);
  public:
    Change(const EigenArray &b, const EigenArray &a, SProperty *prop)
      : SProperty::DataChange(prop),
      _before(b),
      _after(a)
      {
      }
    const EigenArray &before() const { return _before; }
    const EigenArray &after() const { return _after; }

  private:
    EigenArray _before;
    EigenArray _after;
    bool apply(int mode)
      {
      if(mode&Forward)
        {
        ((U*)property())->mData = after();
        property()->postSet();
        }
      else if(mode&Backward)
        {
        ((U*)property())->mData = before();
        property()->postSet();
        }
      if(mode&Inform)
        {
        xAssert(property()->entity());
        property()->entity()->informDirtyObservers(property());
        }
      return true;
      }
    };

  void doChange(const EigenArray &arr)
    {
    void *changeMemory = getChange<Change>();
    Change *change = new(changeMemory) Change(mData, arr, this);
    database()->submitChange(change);
    }

  Eigen::Array <T, Eigen::Dynamic, Eigen::Dynamic> mData;
  };

template <typename T, typename U> void SArrayProperty<T, U>::saveProperty( const SProperty* p_in, SSaver &l)
  {
  SProperty::saveProperty(p_in, l); // saves the data of the parent class (keeps connections)

  const SArrayProperty* ptr = p_in->castTo<SArrayProperty>(); // cast the input property to an SArrayProperty
  xAssert(ptr);
  if(ptr)
    {
    writeValue(l, ptr->mData);
    }
  }

template <typename T, typename U> SProperty *SArrayProperty<T, U>::loadProperty( SPropertyContainer* p_in, SLoader &l)
  {
  SProperty *prop = SProperty::loadProperty(p_in, l);

  SArrayProperty* ptr = prop->uncheckedCastTo<SArrayProperty>();
  xAssert(ptr);
  if(ptr)
    {
    readValue(l, ptr->mData);
    }
  return prop;
  }


class SHIFT_EXPORT SFloatArrayProperty : public SArrayProperty<float, SFloatArrayProperty>
  {
  S_PROPERTY(SFloatArrayProperty, SProperty, 0)
public:
  };

class SHIFT_EXPORT SUIntArrayProperty : public SArrayProperty<xuint32, SUIntArrayProperty>
  {
  S_PROPERTY(SUIntArrayProperty, SProperty, 0)
public:
  };



#endif // SARRAYPROPERTY_H
