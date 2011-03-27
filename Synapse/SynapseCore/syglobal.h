#ifndef SYGLOBAL_H
#define SYGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SYNAPSECORE_BUILD
# define SYNAPSECORE_EXPORT X_DECL_EXPORT
#else
# define SYNAPSECORE_EXPORT X_DECL_IMPORT
#endif

#define SYNAPSE_TYPE(name, id) S_PROPERTY_TYPE(name, 1500, id)

SYNAPSE_TYPE(SyNode, 1);

#endif // SYGLOBAL_H
