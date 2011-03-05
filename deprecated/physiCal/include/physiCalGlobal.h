#ifndef PHYSICALGLOBAL_H
#define PHYSICALGLOBAL_H

#include "QList"
#include "QHash"
#include "jLib.h"

// Export macro
#if WIN32
#   if defined(PHYSICAL_STATIC)
#       define PHYSICAL_EXPORT extern
#   else
#       if defined(PHYSICAL_LIBRARY)
#           define PHYSICAL_EXPORT __declspec(dllexport)
#       else
#           define PHYSICAL_EXPORT __declspec(dllimport)
#       endif
#   endif
#else
#   define PHYSICAL_EXPORT
#endif

// namespace definitions for easy changing
#define BEGIN_PHYSICAL_NAMESPACE namespace cal {

#define END_PHYSICAL_NAMESPACE }

// floating point math accuracy
typedef double pReal;

#include "pVector.h"

// ease of using jLib::math namespace in c++ files
#ifdef PHYSICAL_LIBRARY
using namespace jLib;
using namespace jLib::math;
#endif

BEGIN_PHYSICAL_NAMESPACE

class pVector;
typedef pVector position3;
typedef pVector vector3;

END_PHYSICAL_NAMESPACE
#endif
