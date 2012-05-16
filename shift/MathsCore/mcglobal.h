#ifndef MCGLOBAL_H
#define MCGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTMATHSCORE_BUILD
# define MATHSCORE_EXPORT X_DECL_EXPORT
#else
# define MATHSCORE_EXPORT X_DECL_IMPORT
#endif

#define MCProfileScope 1091
#define MCProfileFunction XProfileFunctionBase(MCProfileScope)
#define MCProfileFunctionScopedBlock(mess) XProfileScopedBlockBase(MCProfileScope, mess)

namespace MathsCore
{
void MATHSCORE_EXPORT initiate();
SPropertyGroup &propertyGroup();
}

#endif // MCGLOBAL_H
