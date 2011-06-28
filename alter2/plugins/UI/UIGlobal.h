#ifndef UIGLOBAL_H
#define UIGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef UI_BUILD
# define UI_EXPORT X_DECL_EXPORT
#else
# define UI_EXPORT X_DECL_IMPORT
#endif

#endif // UIGLOBAL_H
