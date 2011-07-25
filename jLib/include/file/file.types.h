#ifndef JLIB_FILE_TYPES_H
#define JLIB_FILE_TYPES_H

//#include "jLib.global.h"
#include "math/math.h"

#ifndef STRFALSE
#   define STRFALSE -1
#endif

#ifndef FALSE
#   define FALSE false
#endif

#ifndef TRUE
#   define TRUE true
#endif

#define BEGIN_JLIB_FILE_NAMESPACE           \
BEGIN_JLIB_NAMESPACE                        \
namespace file                              \
{

#define END_JLIB_FILE_NAMESPACE             \
}                                           \
END_JLIB_NAMESPACE

BEGIN_JLIB_FILE_NAMESPACE

class ioFile;
class xmlFile;
class filename;
class dir;
class geometryFileAbstract;

END_JLIB_FILE_NAMESPACE

#endif
