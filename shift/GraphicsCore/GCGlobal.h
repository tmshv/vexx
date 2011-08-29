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

#endif // GCGLOBAL_H
