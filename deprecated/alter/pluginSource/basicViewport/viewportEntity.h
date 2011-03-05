#ifndef VIEWPORTENTITY_H
#define VIEWPORTENTITY_H

#include "AEntity"
#include "viewportGlobal.h"

class VIEWPORT_EXPORT viewportEntity : public AEntity
    {
    Q_OBJECT
    jROProperty( jLib::graphics::entity::entityD *, object )
    jROProperty( jLib::graphics::entity::entityG *, group )
    jRefProperty( jLib::graphics::shape::cuboid, bounds )
    jROProperty( bool, visible )
public:
    viewportEntity( );
    ~viewportEntity();

    virtual void setParent( AEntityLocation );
    virtual void update( ) = 0;

    void visible( bool );

    void object( jLib::graphics::entity::entityD * );
    void group( jLib::graphics::entity::entityG * );


private:
    void prePropertyChanged( AProperty * );
    virtual void relocated( );
    };

#endif
