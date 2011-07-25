#include "graphics/entity/graphics.entity.entityG.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_ENTITY_NAMESPACE

entityG::entityG( entityG *p ) : entityD( p )
    {
    setDrawable();
    }

entityG::~entityG( )
    {
    while( _children.size() )
        {
        _children[0]->setParent( 0 );
        }
    }

bool entityG::addChild( entityD &e )
    {
    return addChild( &e );
    }

bool entityG::removeChild( entityD &e )
    {
    return removeChild( &e );
    }

bool entityG::isChild( entityD &e )
    {
    return isChild( &e );
    }

bool entityG::removeChild( entityD *e )
    {
    unsigned int x = child( e );
    if( x < _children.size() )
        {
        _children.erase( _children.begin() + x );
        return TRUE;
        }
    return FALSE;
    }

void entityG::clear()
    {
    for( unsigned int x=0; x<_children.size(); x++ )
        {
        _children[x]->setParent( 0 );
        }
    _children.clear();
    }

bool entityG::addChild( entityD *e )
    {
    jAssert( e != NULL );
    if( isChild( e ) )
        {
        return FALSE;
        }
    _children.push_back( e );
    return TRUE;
    }

entityD *entityG::child( unsigned int x )
    {
    jAssert( x < _children.size() );
    return _children[x];
    }

bool entityG::isChild( entityD *e )
    {
    return child( e ) != _children.size();
    }

unsigned int entityG::size( )
    {
    return _children.size();
    }

unsigned int entityG::child( entityD *e )
    {
    for( unsigned int x=0; x<_children.size(); x++ )
        {
        if( e == _children[x] )
            {
            return x;
            }
        }
    return _children.size();
    }

void entityG::draw()
    {
    for( unsigned int x=0; x<_children.size(); x++ )
        {
        jAssert( child( x )->isDrawable() );
        child( x )->drawGlobal();
        }
    }

END_JLIB_GRAPHICS_ENTITY_NAMESPACE
