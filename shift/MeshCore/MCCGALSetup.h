#ifndef MCCGALSETUP_H
#define MCCGALSETUP_H

#include "QtGlobal"
#include "XAssert"
#include "styperegistry.h"

template <typename T> class CGALAllocator
  {
public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef xsize size_type;
  typedef xptrdiff difference_type;

  template <typename O> struct rebind
    {
    typedef CGALAllocator<O> other;
    };

  pointer allocate(size_type n, void *h=0)
    {
    static XAllocatorBase* alloc = STypeRegistry::allocator();

    (void)h;

    return (pointer)alloc->alloc(sizeof(T)*n);
    }

  void deallocate(pointer p, size_type n)
    {
    static XAllocatorBase* alloc = STypeRegistry::allocator();

    (void)n;
    alloc->free(p);
    }

  void construct(pointer p, const_reference t)
    {
    new(p) T(t);
    }

  void destroy(pointer p)
    {
    (void)p;
    p->~T();
    }
  };

#define CGAL_ALLOCATOR(T) CGALAllocator<T>

#ifdef Q_CC_MSVC
# define CGAL_CFG_NO_NEXTAFTER
#endif
#ifndef X_CPPOX_SUPPORT
# define CGAL_CFG_NO_CPP0X_LONG_LONG
# define CGAL_CFG_NO_CPP0X_RVALUE_REFERENCE
# define CGAL_CFG_NO_CPP0X_TUPLE
# define CGAL_CFG_NO_CPP0X_STATIC_ASSERT
# define CGAL_CFG_NO_CPP0X_NEXT_PREV
# define CGAL_CFG_NO_CPP0X_COPY_N
# define CGAL_CFG_NO_CPP0X_ARRAY
#endif

#ifndef X_CPPOX_VARIADIC_TEMPLATES_SUPPORT
# define CGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES
#endif

#define CGAL_CFG_NO_TR1_ARRAY
#define CGAL_CFG_NO_STATEMENT_EXPRESSIONS
#define CGAL_CFG_NO_TR1_TUPLE
#define CGAL_CFG_ARRAY_MEMBER_INITIALIZATION_BUG
#define BOOST_BIMAP_DISABLE_SERIALIZATION
#define CGAL_NO_AUTOLINK
#define BOOST_THREAD_NO_LIB
#define CGAL_HAS_NO_THREADS
#define CGAL_USE_SSE2_MAX

#endif // MCCGALSETUP_H
