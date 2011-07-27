#ifndef PROPERTYENTITYCREATORS_H
#define PROPERTYENTITYCREATORS_H

#include "QHash"

/** \struct appCreatorFn
  \todo rename this struct
  \brief Holds the data needed to manage and create an entity type.
  */

struct APrivateEntityCreatorFn
    {
    /** The function to call which returns an allocated entity.
      */
    AEntityCreatorFn fn;
    /** Whether the type should be displayed in create menus, or is a private type.
      */
    bool show;
    };

typedef QHash <AEntityType, APrivateEntityCreatorFn> propertyEntityCreators;

#endif // PROPERTYENTITYCREATORS_H
