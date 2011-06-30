#ifndef SAGLOBAL_H
#define SAGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTALTER_BUILD
# define SHIFTALTER_EXPORT X_DECL_EXPORT
#else
# define SHIFTALTER_EXPORT X_DECL_IMPORT
#endif

#endif // SAGLOBAL_H
