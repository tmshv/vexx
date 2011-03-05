#ifndef JLIB_GRAPHICS_PIXEL_IMAGEPLANE_H
#define JLIB_GRAPHICS_PIXEL_IMAGEPLANE_H

#include "graphics/pixel/graphics.pixel.types.h"
#include "graphics/pixel/graphics.pixel.image.h"
#include "graphics/pixel/graphics.pixel.texture.h"
#include "graphics/primitives/graphics.primitives.triangleCloud.h"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

class JLIB_EXPORT imagePlane : public primitives::triangleCloud
    {
public:
    imagePlane( entity::entityG *, image cI=image( 0, 0 ), JFLOAT=1 );
    virtual ~imagePlane( );
    void setFacingCamera( bool _f=TRUE );
    void setParalellCamera( bool _f=TRUE );
    void setImage( image );
    void setWidth( JFLOAT );
    virtual void draw( );
private:
    image currentImage;
    texture *_currentTexture;
    JFLOAT _width;
    bool _facing;
    bool _paralell;
    };

END_JLIB_GRAPHICS_PIXEL_NAMESPACE

#endif
