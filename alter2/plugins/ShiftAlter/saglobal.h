#ifndef SAGLOBAL_H
#define SAGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTALTER_BUILD
# define SHIFTALTER_EXPORT X_DECL_EXPORT
#else
# define SHIFTALTER_EXPORT X_DECL_IMPORT
#endif

S_DEFINE_INTERFACE_TYPE(SPartEditorInterface, 300)

#endif // SAGLOBAL_H
