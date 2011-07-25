#ifndef MATERIALFRONT_H
#define MATERIALFRONT_H

#include "../SDK/propertyEntity.h"
#include "QString"

class materialFront : public propertyEntity
    {
    DEFINE_CLASS_TYPE( "physical.placid-acid.com", "Physics/Material" )
public:
    ~materialFront( );
    static propertyEntity *create( );

    virtual void propertyChanged( QString );

private:
    materialFront( );
    };

#endif
