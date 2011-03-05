#ifndef XADDGLOBAL_H
#define XADDGLOBAL_H

#include "XGlobal"

#if WIN32
#   if defined(EKSADD_STATIC)
#       define EKSADD_EXPORT extern
#   else
#       if defined(EKSADD_BUILD)
#           define EKSADD_EXPORT __declspec(dllexport)
#       else
#           define EKSADD_EXPORT __declspec(dllimport)
#       endif
#   endif
#else
#   define EKSADD_EXPORT
#endif


#endif // XADDGLOBAL_H
