#ifndef SCGLOBAL_H
#define SCGLOBAL_H

#include "XGlobal"

#ifdef SCAPE_BUILD
# define SC_EXPORT //Q_DECL_EXPORT
#else
# define SC_EXPORT //Q_DECL_IMPORT
#endif

#endif // SCGLOBAL_H
