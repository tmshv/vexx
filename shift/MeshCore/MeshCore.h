#ifndef MESHCORE_H
#define MESHCORE_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTMESHCORE_BUILD
# define MESHCORE_EXPORT X_DECL_EXPORT
#else
# define MESHCORE_EXPORT X_DECL_IMPORT
#endif

void MESHCORE_EXPORT initiateMeshCore();

#endif // GRAPHICSCORE_H
