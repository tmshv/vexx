#ifndef MCGLOBAL_H
#define MCGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTMATHSCORE_BUILD
# define MATHSCORE_EXPORT X_DECL_EXPORT
#else
# define MATHSCORE_EXPORT X_DECL_IMPORT
#endif


#define MCProfileScope 1090
#define MCProfileFunction XProfileFunction(MCProfileScope)
#define MCProfileFunctionScopedBlock(mess) XProfileScopedBlock(MCProfileScope, mess)

#endif // MCGLOBAL_H
