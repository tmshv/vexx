#ifndef XSCRIPTGLOBAL_H
#define XSCRIPTGLOBAL_H

#define _ITERATOR_DEBUG_LEVEL 0
#define X_DART

#include "XGlobal"

#ifdef EKSSCRIPT_BUILD
# define EKSSCRIPT_EXPORT X_DECL_EXPORT
#else
# define EKSSCRIPT_EXPORT X_DECL_IMPORT
#endif


#endif // XSCRIPTGLOBAL_H
