#ifndef GCGLOBAL_H
#define GCGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTGRAPHICSCORE_BUILD
# define GRAPHICSCORE_EXPORT X_DECL_EXPORT
#else
# define GRAPHICSCORE_EXPORT X_DECL_IMPORT
#endif

S_DEFINE_INTERFACE_TYPE(ManipulatableInterface, 200)

#define GCProfileScope 1090
#define GCProfileFunction XProfileFunction(GCProfileScope)
#define GCProfileFunctionScopedBlock(mess) XProfileScopedBlock(GCProfileScope, mess)

S_DEFINE_INTERFACE_TYPE(GCShaderBindableInterface, 50)

#endif // GCGLOBAL_H
