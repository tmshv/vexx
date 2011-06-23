#ifndef SARRAYPROPERTY_H
#define SARRAYPROPERTY_H

#include "spropertycontainer.h"
#include "sbaseproperties.h"
#include "XList"
#include "sloader.h"
#include "Eigen/Core"

// reimplement stream for QTextStream to allow it to work with template classes

template <typename T, int U, int V> QTextStream & operator <<(QTextStream &str, const Eigen::Array <T, U, V> &data)
  {
  xsize width = data.cols();
  xsize height = data.rows();
  str << width << " " << height << " ";
  for (xsize i = 0; i < height; ++i)
    {
    for(xsize j = 0; j < width; ++j)
      {
      str << data(i, j);
      if((i < height-1) && (j < width-1)) // while not last element
        {
        str << " "; // separate each element with space
        }
      }
    }
  return str;
  }


template <typename T, int U, int V> QDataStream & operator <<(QDataStream &str, const Eigen::Array <T, U, V> &data)
  {
  xsize width = data.cols();
  xsize height = data.rows();
  str << (quint64) width << (quint64) height;
  for (xsize i = 0; i < height; ++i)
    {
    for(xsize j = 0; j < width; ++j)
      {
      str << data(i, j);
      }
    }
  return str;
  }

template <typename T, int U, int V> QTextStream & operator >>(QTextStream &str, Eigen::Array <T, U, V> &data)
  {
  xsize width;
  xsize height;

  str >> width >> height; // first element in str is size of str
  data.resize(width, height);

  for(xsize i = 0; i < height; ++i )
    {
    for(xsize j = 0; j < width; j++)
      {
      T tVal;
      str >> tVal;
      data(i, j) = tVal;
      }
    }
  return str;
  }

template <typename T, int U, int V> QDataStream & operator >>(QDataStream &str, Eigen::Array <T, U, V> &data)
  {
  quint64 width;
  quint64 height;

  str >> width >> height; // first element in str is size of str
  data.resize(width, height);
  for(xsize i = 0; i < height; ++i )
    {
    for(xsize j = 0; j < width; j++)
      {
      T tVal;
      str >> tVal;
      data(i, j) = tVal;
      }
    }
  return str;
  }


template <typename T, typename U> class SArrayProperty : public SProperty
  {
public:
  typedef T ArrayElementType;
  typedef Eigen::Array <T, Eigen::Dynamic, Eigen::Dynamic> EigenArray;

  const EigenArray &data() const { preGet(); return mData; }

  void add(const SArrayProperty <T, U> *in)
    {
    #warning this should be in a changeFn
    mData += in->data();
    }

  void add(const SArrayProperty <T, U> *inA, const SArrayProperty <T, U> *inB)
    {
    #warning this should be in a changeFn
    mData = inA->data() + inB->data();
    }

  void resize(xsize width, xsize height)
    {
    #warning this should be in a changeFn
    mData.resize(height, width);
    }

  void resize(xsize size)
    {
    #warning this should be in a changeFn
    mData.resize(1, size);
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
    #warning this should be in a changeFn
    resize(width, height);
    if(width != 0 && height != 0)
      {
      memcpy(mData.data(), &val.front(), sizeof(T)*width*height);
      }
    }

  EigenArray &lockData(bool pG=true)
    {
    if(pG)
      {
      preGet();
      }

#warning this needs to cache the old value somewhere...? ref count memory...
    return mData;
    }

  void unlockData()
    {
    #warning this should be in a changeFn
#warning call post set...
    }

  void setIndex(xsize x, xsize y, const T &val)
    {
#warning this should be in a changeFn
    mData(y, x) = val;
    }

  const T &atIndex(xsize x, xsize y) const
    {
    preGet();
    return mData(y, x);
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
