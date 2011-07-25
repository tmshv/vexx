#ifndef JLIB_XML_TYPES_H
#define JLIB_XML_TYPES_H

#include "jLib.global.h"
#include "error/error.h"
#include "jLib.variant.h"

#define BEGIN_JLIB_XML_NAMESPACE            \
BEGIN_JLIB_NAMESPACE                        \
namespace xml                               \
{

#define END_JLIB_XML_NAMESPACE              \
}                                           \
END_JLIB_NAMESPACE

BEGIN_JLIB_XML_NAMESPACE

class attribute;
class attributeList;
class tag;
class tagList;

END_JLIB_XML_NAMESPACE

#endif
