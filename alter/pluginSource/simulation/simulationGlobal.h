#ifndef SIMULATIONGLOBAL_H
#define SIMULATIONGLOBAL_H

#include "qglobal.h"

#if defined(SIMULATION_LIBRARY)
#  define SIMULATION_EXPORT Q_DECL_EXPORT
#else
#  define SIMULATION_EXPORT Q_DECL_IMPORT
#endif

#include "jLib.h"

#endif // SIMULATIONGLOBAL_H
