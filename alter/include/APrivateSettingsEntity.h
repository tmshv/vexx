#ifndef APRIVATESETTINGSENTITY_H
#define APRIVATESETTINGSENTITY_H

#include "../SDK/AEntity"

/** \cond INTERNAL
  */

class APrivateSettingsEntity : public AEntity
    {
    Q_OBJECT
    A_ENTITY( APrivateSettingsEntity, "placid-acid.com", "SettingsEntity" );

public:
    APrivateSettingsEntity();
    };

/** \endcond
  */

#endif // APRIVATESETTINGSENTITY_H
