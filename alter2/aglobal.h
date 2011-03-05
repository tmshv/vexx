#ifndef AGLOBAL_H
#define AGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ALTER_BUILD)
#  define ALTER_EXPORT Q_DECL_EXPORT
#else
#  define ALTER_EXPORT Q_DECL_IMPORT
#endif

#endif // AGLOBAL_H
