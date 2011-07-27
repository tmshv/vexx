#ifndef TIMEGLOBAL_H
#define TIMEGLOBAL_H

#if WIN32
#   if defined(TIME_STATIC)
#       define TIME_EXPORT extern
#   else
#       if defined(TIME_LIBRARY)
#           define TIME_EXPORT __declspec(dllexport)
#       else
#           define TIME_EXPORT __declspec(dllimport)
#       endif
#   endif
#else
#   define TIME_EXPORT
#endif

#include "jLib.h"

#endif // TIMEGLOBAL_H
