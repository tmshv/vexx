#ifndef SGLOBAL_H
#define SGLOBAL_H

#include <QtCore/qglobal.h>
#include "XGlobal"
#include "XList"
#include "XProfiler"

#if defined(SHIFT_BUILD)
#  define SHIFT_EXPORT Q_DECL_EXPORT
#else
#  define SHIFT_EXPORT Q_DECL_IMPORT
#endif

#define ShiftCoreProfileScope 1043
#define ShiftDataModelProfileScope 1044
#define SProfileFunction XProfileFunction(ShiftCoreProfileScope)
#define SProfileScopedBlock(mess) XProfileScopedBlock(ShiftCoreProfileScope, mess)

class SEntity;
class SProperty;
class SObserver;
class STreeObserver;
class SDirtyObserver;
class SConnectionObserver;

typedef XVector<SObserver*> SObservers;
typedef XVector<STreeObserver*> STreeObservers;
typedef XVector<SDirtyObserver*> SDirtyObservers;
typedef XVector<SConnectionObserver*> SConnectionObservers;

typedef XVector<SEntity*> SEntities;
typedef XVector<const SEntity*> SConstEntities;


#endif // SHIFT_GLOBAL_H
