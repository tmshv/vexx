#ifndef JLIB_GRAPHICS_ENTITY_ENTITYD_H
#define JLIB_GRAPHICS_ENTITY_ENTITYD_H

#include "graphics/entity/graphics.entity.types.h"
#include "graphics/entity/graphics.entity.entityB.h"
#include "graphics/shader/graphics.shader.surfaceShader.h"
#include "graphics/shader/graphics.shader.variableSet.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

class JLIB_EXPORT entityD : public entityB
    {
    jProperty( bool, visible )
public:
    entityD( entityG * );
    virtual ~entityD( );

    void setShader( shader::surfaceShader & );
    void clearShader( );
    void setUseShaderVariableSet( bool able=TRUE );
    shader::variableSet &shaderVariableSet();

    void setParent( entityG * );

    math::matrix4x4 transform() const;
    void setTranslate( math::triple );
    void setOrientation( math::quaternion );
    void setScale( math::triple );
    void setPreTransform( const math::matrix4x4 & );
    void setPostTransform( const math::matrix4x4 & );
    math::triple translate( ) const;
    math::triple orientation( ) const;
    math::triple scale( ) const;
    math::matrix4x4 preTransform( );
    math::matrix4x4 postTransform( );
    void setDrawable( bool able=TRUE );
    bool isDrawable();
    void faceCamera( );
    void parallelCamera( );
    virtual void drawGlobal();
    virtual void draw() = 0;
    bBox &getBoundingBox();

private:
    bool _useShader;
    shader::surfaceShader *_shader;
    bool _useShaderVariableSet;
    shader::variableSet _shaderVariableSet;
    bBox *_boundingBox;
    bool _drawable;
    void updateMatrix();
    void pushTransformMatrix();
    void popTransformMatrix();
    math::matrix4x4StackTransform _transform;
    int _aimMatrixRef;
    };

END_JLIB_GRAPHICS_ENTITY_NAMESPACE

#endif
