#ifndef SARRAYPROPERTY_H
#define SARRAYPROPERTY_H

#include "spropertycontainer.h"
#include "sbaseproperties.h"
#include "spropertydata.h"
#include "XList"
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


template <typename T> class SArrayProperty : public SProperty
  {
public:
  typedef Eigen::Array <T, Eigen::Dynamic, Eigen::Dynamic> EigenArray;

  const EigenArray &data() const { preGet(); return mData; }

  void add(const SArrayProperty <T> *in)
    {
    #warning test that rows and cols are the same for both
    mData += in->data();
    postSet();
    }

  void add(const SArrayProperty <T> *inA, const SArrayProperty <T> *inB)
    {
    #warning test that rows and cols are the same for both
    mData = inA->data() + inB->data();
    postSet();
    }

  void resize(xsize width, xsize height)
    {
    mData.resize(height, width);
    postSet();
    }

  void setIndex(xsize x, xsize y, const T &val)
    {
    mData(y, x) = val;
    postSet();
    }

  const T &atIndex(xsize x, xsize y)
    {
    preGet();
    return mData(y, x);
    }

protected:
  // called by parent
  static void SaveFunction( const SProperty* p_in, SPropertyData& data_in, SPropertyData::Mode mode_in); // Mode = Binary / ASCII
  static void LoadFunction( SProperty* p_in, const SPropertyData& data_in, xuint32 v_in, SPropertyData::Mode mode_in, SLoader&); // Mode = Binary / ASCII
  template <typename U> static void AssignFunction(const SProperty *from, SProperty *to)
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

template <typename T> void SArrayProperty<T>::SaveFunction( const SProperty* p_in, SPropertyData& data_in, SPropertyData::Mode mode_in) // Mode = Binary / ASCII
  {
  SProperty::save(p_in, data_in, mode_in); // saves the data of the parent class (keeps connections)

  const SArrayProperty* ptr = p_in->castTo<SArrayProperty>(); // cast the input property to an SArrayProperty
  xAssert(ptr);
  if(ptr)
    {
    QByteArray arr;
    if(mode_in == SPropertyData::Binary) // Binary
      {
      QDataStream str(&arr, QIODevice::WriteOnly); // open a writeonly qstream, writes into arr
      str << ptr->mData;
      }
    else // ASCII
      {
      QTextStream str(&arr, QIODevice::WriteOnly); // writes ascii into arr
      const Eigen::Array <T, Eigen::Dynamic, Eigen::Dynamic> &data = ptr->mData;
      str << data;
      }
    data_in.setValue(arr); // set the data (this is written to disk later)
    }
  }

template <typename T> void SArrayProperty<T>::LoadFunction( SProperty* p_in, const SPropertyData& data_in, xuint32 v_in, SPropertyData::Mode mode_in, SLoader &l_in)
  {
  SProperty::load(p_in, data_in, v_in, mode_in, l_in);

  SArrayProperty* ptr = p_in->castTo<SArrayProperty>();
  xAssert(ptr);
  if(ptr)
    {
    QByteArray arr = data_in.value();
    if(mode_in == SPropertyData::Binary)
      {
      QDataStream str(&arr, QIODevice::ReadOnly);
      str >> ptr->mData;
      }
    else
      {
      QTextStream str(&arr, QIODevice::ReadOnly);
      str >> ptr->mData;
      }
    }
  }



class SHIFT_EXPORT SFloatArrayProperty : public SArrayProperty<float>
  {
  S_PROPERTY(SFloatArrayProperty, SProperty, SaveFunction, LoadFunction, AssignFunction<SFloatArrayProperty>, 0)
public:
  };


#endif // SARRAYPROPERTY_H
