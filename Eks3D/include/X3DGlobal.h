#ifndef X3DGLOBAL_H
#define X3DGLOBAL_H

#include "XGlobal"

#if WIN32
#   if defined(EKS3D_STATIC)
#       define EKS3D_EXPORT extern
#   else
#       if defined(EKS3D_BUILD)
#           define EKS3D_EXPORT __declspec(dllexport)
#       else
#           define EKS3D_EXPORT __declspec(dllimport)
#       endif
#   endif
#else
#   define EKS3D_EXPORT
#endif

#endif // X3DGLOBAL_H
