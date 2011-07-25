#ifndef GRAPHICS_SHADER_LIGHT_H
#define GRAPHICS_SHADER_LIGHT_H

#include "graphics/shader/graphics.shader.types.h"
#include "graphics/entity/graphics.entity.entityB.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

class light : public entity::entityB
    {
    jProperty( math::colour4, ambient )
    jProperty( math::colour4, diffuse )
    jProperty( math::colour4, specular )
    jProperty( math::triple, position )
    jProperty( math::triple, direction )
    jMBProperty( JFLOAT, coneAngle )
    jProperty( JFLOAT, coneCosFallOff )
    jProperty( JFLOAT, constantAttenuation )
    jProperty( JFLOAT, linearAttenuation )
    jProperty( JFLOAT, quadraticAttenuation )
public:
    light( entity::entityG * );

    JFLOAT coneAngle()
        {
        return acos( _coneAngle );
        }
    void coneAngle( JFLOAT in )
        {
        _coneAngle = cos( in );
        }
    };

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
