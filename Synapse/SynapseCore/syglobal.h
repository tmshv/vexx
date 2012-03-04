#ifndef SYGLOBAL_H
#define SYGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SYNAPSECORE_BUILD
# define SYNAPSECORE_EXPORT X_DECL_EXPORT
#else
# define SYNAPSECORE_EXPORT X_DECL_IMPORT
#endif

#endif // SYGLOBAL_H
