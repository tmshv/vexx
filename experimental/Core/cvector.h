#ifndef CVECTOR_H
#define CVECTOR_H

#include "ccore.h"
#include "callocator.h"

template <typename Element, typename SizeType, typename Allocator=CAllocator<Element,SizeType> > class CVector
  {
public:
  CVector( ) : _internal(0)
    {
    }

  CVector( SizeType size ) : _internal(new CVectorPrivate)
    {
    gI()->mReferences = 1;
    gI()->reserve(size);
    }

  CVector( const CVector &cpy ) : _internal(cpy._internal)
    {
    ++gI()->mReferences;
    }

  ~CVector( )
    {
    --gI()->mReferences;
    if(gI()->mReferences == 0)
      {
      delete _internal;
      }
    }


  void resize( SizeType size )
    {
    gI()->reserve(size);
    gI()->mUsed = &(gI()->mBegin[size]);
    }

  void resize( SizeType size, const Element &elem )
    {
    gI()->reserve(size);
    gI()->mUsed = &(gI()->mBegin[size]);

    for(SizeType i=0; i<size; ++i)
      {
      gI()->mBegin[i] = elem;
      }
    }

  void reserve( SizeType size )
    {
    gI()->reserve(size);
    }


  SizeType size() const
    {
    return gI() ? (gI()->size()) : 0;
    }

  SizeType capacity() const
    {
    return gI()->mEnd - gI()->mBegin;
    }


  Element &at( SizeType in )
    {
    CASSERT(_internal);
    CASSERT(in<size());
    return gI()->mBegin[in];
    }

  const Element &at( SizeType in ) const
    {
    CASSERT(_internal);
    CASSERT(in<size());
    return gI()->mBegin[in];
    }


  Element &operator[]( SizeType in )
    {
    return at(in);
    }

  const Element &operator[]( SizeType in ) const
    {
    return at(in);
    }

private:
  class CVectorPrivate
    {
  public:
    CVectorPrivate() : mBegin(0), mUsed(0), mEnd(0), mReferences(0) { }
    ~CVectorPrivate()
      {
      Allocator::release(mBegin);
      mBegin = mUsed = mEnd = 0;
      }

    SizeType size() const
      {
      return mUsed-mBegin;
      }
    void reserve(SizeType newReserve)
      {
      SizeType s = size();
      Element *newBegin = Allocator::get(newReserve);

      if(mBegin)
        {
        for(SizeType i=0; i<s; ++i)
          {
          newBegin[i] = mBegin[i];
          }
        }

      mBegin = newBegin;
      mUsed = &(mBegin[s]);
      mEnd = &(mBegin[newReserve]);
      }

    Element *mBegin;
    Element *mUsed;
    Element *mEnd;
    unsigned int mReferences;
    };

  CVectorPrivate *gI()
    {
    if(_internal->mReferences > 1)
      {
      CVectorPrivate *oI = _internal;
      _internal = new CVectorPrivate;
      _internal->reserve(oI->size());
      
      SizeType s = oI->size();
      for(SizeType i=0; i<s; ++i)
        {
        _internal->mBegin[i] = Element(oI->mBegin[i]);
        }
      
      --_internal->mReferences;
      }
    return _internal;
    }

  const CVectorPrivate *gI() const
    {
    return _internal;
    }
  CVectorPrivate *_internal;
  };

#endif // CVECTOR_H
