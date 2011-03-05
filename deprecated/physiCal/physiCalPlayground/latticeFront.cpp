#include "../SDK/propertyEntityArray.h"
#include "latticeFront.h"

propertyEntity *latticeFront::create( )
    {
    return new latticeFront( );
    }

latticeFront::latticeFront( ) : viewportEntity( ), _mesh( new jLib::graphics::primitives::triangleArray( 0 ) )
    {
    object( _mesh.getTriangleArray() );
    addProperty( "lattice/file", "", "Mesh File" );
    addProperty( "lattice/material", "", "Material" );
    }

void latticeFront::propertyChanged( QString name )
    {
    if( name == "lattice/file" )
        {
        if( jLib::file::ioFile::exists( property( name )->toString() ) )
            {
            jLib::graphics::collada::file collada( jLib::file::ioFile( "torusThing.dae" ) );
            jLib::vector <jLib::graphics::collada::geometry> geos = collada.get<jLib::graphics::collada::geometry>( jLib::graphics::collada::base::geometry );
            _mesh.clearPolygonArray();
            if( geos.size() && geos[0].size() )
                {
                geos[0].get( 0, _mesh );
                bounds() = _mesh.getTriangleArray()->bounds();
                }
            else
                {
                property( "lattice/file" ) = "";
                appLog<<"No appropriate geometry found in file."<<endl;
                }
            }
        else
            {
            if( property( "lattice/file" )->toString() != "" )
                {
                appLog<<"That file doesnt exist"<<endl;
                property( "lattice/file" ) = "";
                }
            }
        setPropertyChanged();
        }
    else
        {
        viewportEntity::propertyChanged( name );
        }
    }
