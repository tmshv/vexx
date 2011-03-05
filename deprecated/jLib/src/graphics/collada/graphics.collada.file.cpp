#include "graphics/collada/graphics.collada.file.h"

BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE

file::file( string in )
    {
    setContent( in );
    base::setMyRoot( &root );
    }

file::file( jLib::file::ioFile in )
    {
    setContent( in.get() );
    base::setMyRoot( &root );
    }

file::~file()
    {
    }

void file::setContent( string in )
    {
    content = xml::strip( in );
    root = xml::xmlString( content ).fromString().findFirst( "COLLADA" );
    }

unsigned int file::size( type t )
    {
    xml::tag element = getMyRoot()->findFirst( getFindString( t ) );

    return element.findAll( getSubFindString( t ) ).size();
    }

bool file::has( type t )
    {
    return size( t ) > 0;
    }

string file::getFindString( type t )
    {
    switch( t )
        {
        case geometry:
            return "library_geometries";
        break;
        default:
            jFail( "Unrecognised element type" );
        break;
        }
    return "";
    }

string file::getSubFindString( type t )
    {
    switch( t )
        {
        case geometry:
            return "geometry";
        break;
        default:
            jFail( "Unrecognised element type" );
        break;
        }
    return "";
    }

END_JLIB_GRAPHICS_COLLADA_NAMESPACE

