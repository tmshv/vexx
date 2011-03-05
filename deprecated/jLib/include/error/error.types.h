#ifndef JLIB_ERROR_TYPES_H
#define JLIB_ERROR_TYPES_H

#include "jLib.global.h"

#define JLIB_ERROR_DO_NOTHING static_cast<void>

#if defined Q_OS_LINUX && __GNUC__
#   ifdef OSX
#       define JLIB_CURRENT_FUNCTION "_No Function Info Available_"
#   else
#       if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
#           define JLIB_CURRENT_FUNCTION	__PRETTY_FUNCTION__
#       else
#           if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#               define JLIB_CURRENT_FUNCTION	__func__
#           else
#               define JLIB_CURRENT_FUNCTION	"_No Function Info Available_"
#           endif
#       endif
#   endif
#else
#   define JLIB_CURRENT_FUNCTION "_No Function Info Available_"
#endif

#ifndef __STRING
#   define __STRING(x) #x
#endif

#define BEGIN_JLIB_ERROR_NAMESPACE           \
BEGIN_JLIB_NAMESPACE                        \
namespace error                              \
{

#define END_JLIB_ERROR_NAMESPACE             \
}                                           \
END_JLIB_NAMESPACE

#endif
