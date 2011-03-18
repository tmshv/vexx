#ifndef SAGLOBAL_H
#define SAGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTALTER_BUILD
# define SHIFTALTER_EXPORT X_DECL_EXPORT
#else
# define SHIFTALTER_EXPORT X_DECL_IMPORT
#endif

#define SHIFTALTER_TYPE(name, id) S_PROPERTY_TYPE(name, 1000, id)

SHIFTALTER_TYPE(SAppDatabase, 1);

#endif // SAGLOBAL_H
