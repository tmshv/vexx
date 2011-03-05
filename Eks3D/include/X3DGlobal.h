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

#if xReal == float
#define EKS_XREAL_GL_TYPE GL_FLOAT
#else
#define EKS_XREAL_GL_TYPE GL_DOUBLE
#endif

typedef xuint64 XEnvironmentID;

enum TextureFormat
  {
  RGB = 1,
  RGBA = 2,
  Byte = 4,
  Short = 8,
  Half = 16,
  Float = 32,
  };

#endif // X3DGLOBAL_H
