#ifndef APPBASEGLOBAL_H
#define GLOBAL_H

#include "XMessageRouter"
#include "XProperty"

#if WIN32
#   if defined(ALTER_STATIC)
#       define ALTER_EXPORT extern
#   else
#       if defined(ALTER_LIBRARY)
#           define ALTER_EXPORT __declspec(dllexport)
#       else
#           define ALTER_EXPORT __declspec(dllimport)
#       endif
#   endif
#else
#   define ALTER_EXPORT
#endif

#endif
