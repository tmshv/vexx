#ifndef JLIB_GRAPHICS_ENTITY_ENTITYG_H
#define JLIB_GRAPHICS_ENTITY_ENTITYG_H

#include "graphics/entity/graphics.entity.types.h"
#include "graphics/entity/graphics.entity.entityD.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

class JLIB_EXPORT entityG : public entityD
    {
public:
    entityG( entityG * );
    virtual ~entityG( );

    bool addChild( entityD & );
    bool addChild( entityD * );
    bool removeChild( entityD & );
    bool removeChild( entityD * );
    entityD *child( unsigned int );
    bool isChild( entityD * );
    bool isChild( entityD & );
    unsigned int size( );
    virtual void draw();
    void clear();
private:
    vector <entityD *> _children;
    unsigned int child( entityD * );
    };

END_JLIB_GRAPHICS_ENTITY_NAMESPACE

#endif
