#include "graphics/entity/graphics.entity.entityD.h"
#include "graphics/entity/graphics.entity.entityG.h"
#include "graphics/entity/graphics.entity.bBox.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

entityD::entityD( entityG *p ) : entityB( p ), _visible( TRUE ), _useShader( FALSE ),
    _useShaderVariableSet( FALSE ), _boundingBox( 0 ), _aimMatrixRef( -1 )
    {
    if( !isTop() )
        {
        parent()->addChild( this );
        }
    }

entityD::~entityD( )
    {
    if( !isTop() )
        {
        parent()->removeChild( this );
        }
    if( _boundingBox != 0 )
        {
        delete _boundingBox;
        }
    }

void entityD::setShader( shader::surfaceShader &in )
    {
    _useShader = true;
    _shader = &in;
    }

void entityD::clearShader( )
    {
    _shader = FALSE;
    _useShader = FALSE;
    }

void entityD::setUseShaderVariableSet( bool able )
    {
    _useShaderVariableSet = able;
    }

void entityD::setParent( entityG *p )
    {
    if( parent() )
        {
        parent()->removeChild( this );
        }
    entityB::setParent( p );
    if( parent() )
        {
        parent()->addChild( this );
        }
    }

math::matrix4x4 entityD::transform() const
    {
    return _transform.resultant();
    }

shader::variableSet &entityD::shaderVariableSet()
    {
    return _shaderVariableSet;
    }

void entityD::setTranslate( math::triple t )
    {
   _transform.setTranslate( t );
    }

void entityD::setOrientation( math::quaternion t )
    {
    _transform.setOrientation( t );
    }

void entityD::setScale( math::triple t )
    {
    _transform.setScale( t );
    }

void entityD::setPreTransform( const math::matrix4x4 &t )
    {
    _transform.setPreTransform( t );
    }

void entityD::setPostTransform( const math::matrix4x4 &t )
    {
    _transform.setPostTransform( t );
    }

math::matrix4x4 entityD::preTransform( )
    {
    return _transform.preTransform( );
    }

math::matrix4x4 entityD::postTransform( )
    {
    return _transform.postTransform( );
    }

math::triple entityD::translate( ) const
    {
    return _transform.translate();
    }

math::triple entityD::orientation( ) const
    {
    return _transform.orientation();
    }

math::triple entityD::scale( ) const
    {
    return _transform.scale();
    }


void entityD::setDrawable( bool able )
    {
    _drawable = able;
    }

bool entityD::isDrawable()
    {
    return _drawable;
    }

void entityD::faceCamera( )
    {
    camera()->update();

    math::triple aimZ = ( camera()->translate - translate() ).normalise();
    math::triple aimY = camera()->V.normalise();
    math::triple aimX = ( aimY.crossProduct( aimZ ) ).normalise();

//#warning aim matrix

    math::matrix4x4 mat = math::matrix4x4::IDENTITY;

    mat.setXAxis( aimX );
    mat.setYAxis( aimY );
    mat.setZAxis( aimZ );

    mat.multiplyCurrent();

    /*math::matrix4x4 mat;( aimX.x(), aimX.y(), aimX.z(), 0,
                            aimY.x(), aimY.y(), aimY.z(), 0,
                            aimZ.x(),  aimZ.y(),  aimZ.z(),
                            0, 0, 0, 0, 1 );*/

    if( _aimMatrixRef == -1 )
        {
        _aimMatrixRef = _transform.insert( _transform.translateIndex(), mat );
        }
    else
        {
        _transform[_aimMatrixRef] = mat;
        }
    _transform.refresh();
    }

void entityD::parallelCamera( )
    {
    camera()->update();

    math::triple aimZ = ( camera()->translate - camera()->direction ).normalise();
    math::triple aimY = camera()->V.normalise();
    math::triple aimX = ( aimY.crossProduct( aimZ ) ).normalise();

//#warning aim matrix
    math::matrix4x4 mat = math::matrix4x4::IDENTITY;

    mat.setXAxis( aimX );
    mat.setYAxis( aimY );
    mat.setZAxis( aimZ );

    mat.multiplyCurrent();
    //math::matrix4x4 mat;//( aimX.x(), aimX.y(), aimX.z(), 0, aimY.x(), aimY.y(), aimY.z(), 0, aimZ.x(),  aimZ.y(),  aimZ.z(), 0, 0, 0, 0, 1 );

    if( _aimMatrixRef == -1 )
        {
        _aimMatrixRef = _transform.insert( _transform.translateIndex(), mat );
        }
    else
        {
        _transform[_aimMatrixRef] = mat;
        }
    _transform.refresh();
    }

void entityD::pushTransformMatrix()
    {
    glPushMatrix();
    _transform.resultant().multiplyCurrent();
    }

void entityD::popTransformMatrix()
    {
    glPopMatrix();
    }

void entityD::drawGlobal()
    {
    if( _visible )
        {
        jRenderDebug( "Render object "<<this );
        if( _useShader )
            {
            jRenderDebug( "Set shader to "<<_shader );
            jAssert( _shader );
            _shader->use();
            }
        if( _useShaderVariableSet )
            {
            jRenderDebug( "Set Variable set" );
            _shaderVariableSet.setValues();
            }
        pushTransformMatrix();
        draw();
        popTransformMatrix();
        if( _useShaderVariableSet )
            {
            jRenderDebug( "Set Variable set" );
            _shaderVariableSet.unsetValues();
            }
        if( _useShader )
            {
            jRenderDebug( "Clear Shader" );
            _shader->clear();
            }
        }
    }

bBox &entityD::getBoundingBox()
    {
    if( _boundingBox == NULL )
        {
        _boundingBox = new bBox( );
        }
    return *_boundingBox;
    }

END_JLIB_GRAPHICS_ENTITY_NAMESPACE
