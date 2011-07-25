#ifndef JLIB_GLOBAL_H
#define JLIB_GLOBAL_H

//Basic include files
#include <stdio.h>
#include <stdlib.h>
#include <ostream>
#include <sstream>
#include <fstream>
//#include <vector>
//#include <map>
#include <math.h>
//#include <string>
//#include <string.h>
#include <limits>
#include <typeinfo>

#if WIN32
#   if defined(JLIB_BUILD)
#       define JLIB_EXPORT __declspec(dllexport)
#   else
#       define JLIB_EXPORT __declspec(dllimport)
#   endif
#else
#   define JLIB_EXPORT
#endif

#define JLIB_POST_ALIGN( x )
#define JLIB_PRE_ALIGN( x ) __attribute__((aligned( x )))

#define JLIB_TYPEOF( typ ) __typeof__( typ )

#define FOREACH( vec, it ) \
    typedef JLIB_TYPEOF( vec ) _CONTAINTERTYPE; \
    for( _CONTAINTERTYPE::iterator it = (vec).begin(); it != (vec).end(); ++it )

#define FOREACHCONST( vec, it ) \
    typedef JLIB_TYPEOF( vec ) _CONTAINTERTYPE; \
    for( _CONTAINTERTYPE::const_iterator it = (vec).begin(); it != (vec).end(); ++it )

#define BEGIN_JLIB_NAMESPACE                \
namespace jLib                              \
{

#define END_JLIB_NAMESPACE                  \
}


#define JLIB_VECTOR_CHILD( type, name )             \
public:                                             \
    typedef  type::iterator iterator;               \
    typedef  type::const_iterator const_iterator;   \
    iterator begin( )                               \
        { return name . begin(); }                  \
    const_iterator begin( ) const                   \
        { return name . begin(); }                  \
    iterator end( )                                 \
        { return name . end(); }                    \
    const_iterator end( ) const                     \
        { return name . end(); }                    \

#ifndef TRUE
#   define TRUE (!(FALSE))
#endif
#ifndef FALSE
#   define FALSE 0
#endif


#define JLIB_QT_SUPPORT

// For future memory pool type use, and now for debugging and finding leaks...
void *operator new( size_t size );
void *operator new[]( size_t size );
void operator delete( void *ptr );
void operator delete[]( void *ptr );

typedef float JFLOAT;
//#define JFLOAT_IS_DOUBLE
typedef int JINT;
typedef bool JBOOL;

#include "jLib.vector.h"
#include "jLib.list.h"
#include "jLib.map.h"
#include "jLib.properties.h"
#include "jLib.floatLimits.h"
#include "jLib.string.h"
#include "jLib.jptr.h"

#endif
