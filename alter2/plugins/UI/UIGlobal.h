#ifndef UIGLOBAL_H
#define UIGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef UI_BUILD
# define UI_EXPORT X_DECL_EXPORT
#else
# define UI_EXPORT X_DECL_IMPORT
#endif

#define UI_TYPE(name, id) S_PROPERTY_TYPE(name, 10000, id)

UI_TYPE(UIWorkspace, 0);
UI_TYPE(UILayout, 1);
UI_TYPE(UIHorizontalLayout, 2);
UI_TYPE(UIVerticalLayout, 3);
UI_TYPE(UIStackedLayout, 4);

#endif // UIGLOBAL_H
