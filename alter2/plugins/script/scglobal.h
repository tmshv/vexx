#ifndef SCRIPT_GLOBAL_H
#define SCRIPT_GLOBAL_H

#include <QtCore/qglobal.h>
#include "XProfiler"

#if defined(SCRIPT_BUILD)
# define SCRIPT_EXPORT Q_DECL_EXPORT
#else
# define SCRIPT_EXPORT Q_DECL_IMPORT
#endif

#define ScriptProfileScope 502
#define ScProfileFunction XProfileFunction(ScriptProfileScope)
#define ScProfileScopedBlock(mess) XProfileScopedBlock(ScriptProfileScope, mess)

#endif // SCRIPT_GLOBAL_H
