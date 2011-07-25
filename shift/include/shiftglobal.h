#ifndef SHIFTGLOBAL_H
#define SHIFTGLOBAL_H

#include "QtGlobal"

#ifdef SHIFT_BUILD
# define SHIFT_EXPORT Q_DECL_EXPORT
#else
#define SHIFT_IMPORT Q_DECL_IMPORT
#endif

#endif // SHIFTGLOBAL_H
