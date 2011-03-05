#ifndef JLIB_GRAPHICS_ENTITY_ENTITYB_H
#define JLIB_GRAPHICS_ENTITY_ENTITYB_H

#include "graphics/entity/graphics.entity.types.h"
#include "graphics/camera/graphics.camera.types.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

class JLIB_EXPORT entityB
    {
public:
    entityB( entityG * );
    virtual ~entityB();
    //! Returns true if this entity is the top of a chain of entities
    bool isTop();
    //! Sets the entitys parent ( 0 for the top of a chain )
    virtual void setParent( entityG * );
    //! Returns this entitys parent ( 0 for the top of a chain )
    entityG *parent();
    //! Returns this entitys parent ( 0 for the top of a chain )
    const entityG *parent() const;
    //! Attempts to return the view at the top of the chain, 0 on a malformed chain
    entityG *root();
    //! Attempts to return the view at the top of the chain, 0 on a malformed chain
    const entityG *root() const;
    //! Attempts to return the camera currently ascociated with this entity
    camera::base *camera();
    //! Attempts to return the camera currently ascociated with this entity
    const camera::base *camera() const;
private:
    entityG *_parent;
    camera::base *_currentCamera;
    friend class render::scene;
    };

END_JLIB_GRAPHICS_ENTITY_NAMESPACE

#include "graphics/camera/graphics.camera.base.h"

#endif
