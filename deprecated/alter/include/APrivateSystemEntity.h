#ifndef SYSTEMENTITY_H
#define SYSTEMENTITY_H

#include "../SDK/AEntity"

/** \cond INTERNAL
  */

class ALTER_EXPORT APrivateSystemEntity : public AEntity
    {
    Q_OBJECT
    A_ENTITY( APrivateSystemEntity, "placid-acid.com", "system" )
public:
    APrivateSystemEntity();
    void setData( QString, QString, QString, QString );
    };

/** \endcond
  */

#endif
