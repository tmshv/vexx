#include "objectEntity.h"

#include "ALog"

objectEntity::objectEntity() : _mesh( new jLib::graphics::primitives::triangleArray( 0 ) )
    {
    // add a property to test for inherited object types
    addProperty( "object/isObject", TRUE );
    property( "object/isObject" ).hide();

    // add Gui properties
    addProperty( "file", "", propertyInfo::File, "File" );
    addProperty( "material", "", propertyInfo::String, "Material" );
    addProperty( "position", 0.0, propertyInfo::Position, "Position" );
    addProperty( "velocity", 0.0, propertyInfo::Vector, "Velocity" );
    addProperty( "randomPoints", 0, propertyInfo::Int, "Random Points" );
    addProperty( "randomSprings", 0, propertyInfo::Int, "Random Springs" );
    addProperty( "maxLength", 1.0, propertyInfo::Double, "Maximum Internal Spring Length" );

    // set up draw mesh
    object( _mesh.getTriangleArray() );
    }

void objectEntity::update()
    {
    // test file exists
    if( jLib::file::ioFile::exists( property( "file" )->toString().toStdString() ) )
        {
        // try to create a collada file
        jLib::graphics::collada::file colFile( jLib::file::ioFile( property( "file" )->toString().toStdString() ) );

        // strip out the first geometry object
        jLib::vector <jLib::graphics::collada::geometry> geos = colFile.get<jLib::graphics::collada::geometry>( jLib::graphics::collada::base::geometry );

        // extract it into a mesh
        if( geos[0].size() )
            {
            geos[0].get( 0, _mesh );
            }
        }
    else
        {
        // otherwise draw nothing
        _mesh.clearPolygonArray();
        _mesh.rebuild();
        }
    // update mesh translate from property
    _mesh.getTriangleArray()->setTranslate( property( "position" )->toVector3D() );
    }
