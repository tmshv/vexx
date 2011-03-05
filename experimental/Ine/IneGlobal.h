#ifndef INEGLOBAL_H
#define INEGLOBAL_H

#include "XGlobal"

#ifdef INE_BUILD
# define INE_EXPORT X_DECL_EXPORT
#else
# define INE_EXPORT X_DECL_IMPORT
#endif

#endif // INEGLOBAL_H
