#include "graphics/entity/graphics.entity.entityB.h"
#include "jLib.glHeader.h"
#include "graphics/entity/graphics.entity.entityG.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

entityB::entityB( entityG *p ) : _currentCamera( 0 )
    {
    _parent = p;
    }

entityB::~entityB()
    {
    }

bool entityB::isTop()
    {
    return _parent == 0;
    }

void entityB::setParent( entityG *p )
    {
    _parent = p;
    }

entityG *entityB::parent()
    {
    return _parent;
    }

entityG *entityB::root()
    {
    if( _parent )
        {
        return _parent->root();
        }

    jAssert( dynamic_cast <entityG *> (this) );
    return static_cast <entityG *> (this);
    }

camera::base *entityB::camera()
    {
    jAssert( root()->_currentCamera );
    return root()->_currentCamera;
    }

const camera::base *entityB::camera() const
    {
    jAssert( _currentCamera );
    return _currentCamera;
    }

const entityG *entityB::parent() const
    {
    return _parent;
    }

const entityG *entityB::root() const
    {
    if( _parent )
        {
        return _parent->root();
        }

    jAssert( dynamic_cast <const entityG *> (this) );
    return static_cast <const entityG *> (this);
    }

END_JLIB_GRAPHICS_ENTITY_NAMESPACE
