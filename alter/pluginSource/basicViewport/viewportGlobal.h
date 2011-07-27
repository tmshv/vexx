#ifndef VIEWPORTGLOBAL_H
#define VIEWPORTGLOBAL_H

#include "jLib.h"

#if WIN32
#   if defined(VIEWPORT_STATIC)
#       define VIEWPORT_EXPORT extern
#   else
#       if defined(VIEWPORT_LIBRARY)
#           define VIEWPORT_EXPORT __declspec(dllexport)
#       else
#           define VIEWPORT_EXPORT __declspec(dllimport)
#       endif
#   endif
#else
#   define VIEWPORT_EXPORT
#endif

#endif // VIEWPORTGLOBAL_H
