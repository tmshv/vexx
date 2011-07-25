#include "graphics/collada/graphics.collada.geometry.h"

BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE

template <typename T> void addSource( xml::tag &tag, string name, string indexNames, const primitives::polygonMeshDataCache <T> &cache )
    {
    if( cache.size() )
        {
        xml::tag &source( tag.add( xml::tag( "source" ) ) );
        source.attributes().add( xml::attribute( "id", name ) );

        xml::tag &array( source.add( xml::tag( "float_array" ) ) );
        array.attributes().add( xml::attribute( "id", name+"-array" ) );
        array.attributes().add( xml::attribute( "count", cache.get( 0 ).size() * cache.size() ) );

        std::ostringstream output;
        for( unsigned int x=0; x<cache.size(); x++ )
            {
            const T &element( cache.get(x) );
            for( unsigned int y=0; y<element.size(); y++ )
                {
                output << element[y];
                if( y < ( element.size() - 1 ) )
                    {
                    output << " ";
                    }
                }
            output << "\n";
            }
        array.add( output.str() );

        xml::tag &accessor( source.add( xml::tag( "technique_common" ) ).add( xml::tag( "accessor" ) ) );
        accessor.attributes().add( xml::attribute( "source", "#" + name + "-array" ) );

        accessor.attributes().add( xml::attribute( "stride", (unsigned int)indexNames.length() ) );
        accessor.attributes().add( xml::attribute( "count", cache.size() ) );
        for( unsigned int x=0; x<indexNames.length(); x++ )
            {
            xml::tag &param( accessor.add( xml::tag( "param" ) ) );
            param.attributes().add( xml::attribute( "name", indexNames[x] ) );
            param.attributes().add( xml::attribute( "type", "float" ) );
            }
        }
    }

string meshToCollada( const primitives::polygonMesh &in )
    {
    xml::tag root( "COLLADA" );

    //Set attributes
        {
        xml::attributeList &rootAttrs( root.attributes() );
        rootAttrs.add( xml::attribute( "xmlns", "http://www.collada.org/2005/11/COLLADASchema" ) );
        rootAttrs.add( xml::attribute( "version", "1.4.1" ) );
        }

    //Add Asset tags
        {
        xml::tag &asset( root.add( xml::tag( "asset") ) );
        asset.add( xml::tag( "contributor" ) ).type( xml::tag::Empty );
        asset.add( xml::tag( "created" ) ).add( "2000-01-01T12:00:00" );
        asset.add( xml::tag( "modified" ) ).add( "2000-01-01T12:00:00" );
        }

    string geomId( "geo" );

    //Add a material
        {
        xml::tag &effect( root.add( xml::tag( "library_effects" ) ).add( xml::tag( "effect") ) );
        effect.attributes().add( xml::attribute( "id", "whitePhong" ) );
        xml::tag &technique( effect.add( xml::tag( "profile_COMMON" ) ).add( xml::tag( "technique" ) ) );
        technique.attributes().add( xml::attribute( "sid", "phong1" ) );
        xml::tag &phong( technique.add( xml::tag( "phong" ) ) );
        phong.add( xml::tag( "emission" ) ).add( xml::tag( "color" ) ).add( "1.0 1.0 1.0 1.0" );
        phong.add( xml::tag( "ambient" ) ).add( xml::tag( "color" ) ).add( "1.0 1.0 1.0 1.0" );
        phong.add( xml::tag( "diffuse" ) ).add( xml::tag( "color" ) ).add( "1.0 1.0 1.0 1.0" );
        phong.add( xml::tag( "specular" ) ).add( xml::tag( "color" ) ).add( "1.0 1.0 1.0 1.0" );
        phong.add( xml::tag( "shininess" ) ).add( xml::tag( "float" ) ).add( "20.0" );
        phong.add( xml::tag( "reflective" ) ).add( xml::tag( "color" ) ).add( "1.0 1.0 1.0 1.0" );
        phong.add( xml::tag( "reflectivity" ) ).add( xml::tag( "float" ) ).add( "0.5" );
        phong.add( xml::tag( "transparent" ) ).add( xml::tag( "color" ) ).add( "1.0 1.0 1.0 1.0" );
        phong.add( xml::tag( "transparency" ) ).add( xml::tag( "float" ) ).add( "1.0" );

        xml::tag &material( root.add( xml::tag( "library_materials" ) ).add( xml::tag( "material" ) ) );
        material.attributes().add( xml::attribute( "id", "whiteMaterial" ) );
        material.add( xml::tag( "instance_effect" ) ).attributes().add( xml::attribute( "url", "#whitePhong" ) );
        }

    //Add Geometry flags...
        {
        xml::tag &libGeo( root.add( xml::tag( "library_geometries" ) ) );
        xml::tag &geo( libGeo.add( xml::tag( "geometry" ) ) );
        geo.attributes().add( xml::attribute( "id", geomId ) );
        geo.attributes().add( xml::attribute( "name", geomId ) );
        xml::tag &mesh( geo.add( xml::tag( "mesh" ) ) );

        addSource( mesh, geomId + "-position", "XYZ", in.points );
        addSource( mesh, geomId + "-normals", "XYZ", in.normals );
        addSource( mesh, geomId + "-uv", "UV", in.textures );

        //Add the indicies
            {
            xml::tag &verticies( mesh.add( xml::tag( "vertices" ) ) );
            verticies.attributes().add( xml::attribute( "id", geomId + "-vertices" ) );
            xml::tag &verticiesInput( verticies.add( xml::tag( "input" ) ) );
            verticiesInput.attributes().add( xml::attribute( "semantic", "POSITION" ) );
            verticiesInput.attributes().add( xml::attribute( "source", "#" + geomId + "-position" ) );

            xml::tag &polys( mesh.add( xml::tag( "polygons" ) ) );
            polys.attributes().add( xml::attribute( "count", in.sizePolygonArray() ) );
            polys.attributes().add( xml::attribute( "material", "WHITE" ) );
            //Add Inputs
                {
                xml::tag &vInput( polys.add( xml::tag( "input" ) ) );
                vInput.attributes().add( xml::attribute( "semantic", "VERTEX" ) );
                vInput.attributes().add( xml::attribute( "source", "#" + geomId + "-vertices" ) );
                vInput.attributes().add( xml::attribute( "offset", 0 ) );

                xml::tag &nInput( polys.add( xml::tag( "input" ) ) );
                nInput.attributes().add( xml::attribute( "semantic", "NORMAL" ) );
                nInput.attributes().add( xml::attribute( "source", "#" + geomId + "-normals" ) );
                nInput.attributes().add( xml::attribute( "offset", 1 ) );

                xml::tag &tInput( polys.add( xml::tag( "input" ) ) );
                tInput.attributes().add( xml::attribute( "semantic", "TEXCOORD" ) );
                tInput.attributes().add( xml::attribute( "source", "#" + geomId + "-uv" ) );
                tInput.attributes().add( xml::attribute( "offset", 2 ) );
                }

            for( unsigned int x=0; x<in.sizePolygonArray(); x++ )
                {
                std::ostringstream output;
                for( unsigned int y=0; y<in.sizePolygon( x ); y++ )
                    {
                    output << in.getPolygonVertexPoint( x, y ) << " "
                           << in.getPolygonVertexNormal( x, y ) << " "
                           << in.getPolygonVertexTexture( x, y );
                    if( y < ( in.sizePolygon(x) - 1 ) )
                        {
                        output << " ";
                        }
                    }
                polys.add( xml::tag( "p" ) ).add( output.str() );
                }
            }
        }

    //Add visual scene
        {
        xml::tag &scene( root.add( xml::tag( "library_visual_scenes" ) ).add( xml::tag( "visual_scene" ) ) );
        scene.attributes().add( xml::attribute( "id", "DefaultScene" ) );
        xml::tag &node( scene.add( xml::tag( "node" ) ) );
        node.attributes().add( xml::attribute( "id", geomId ) );
        node.attributes().add( xml::attribute( "name", geomId ) );
        node.add( xml::tag( "translate" ) ).add( "0 0 0" );
        node.add( xml::tag( "rotate" ) ).add( "0 0 1 0" );
        node.add( xml::tag( "rotate" ) ).add( "0 1 0 0" );
        node.add( xml::tag( "rotate" ) ).add( "1 0 0 0" );
        node.add( xml::tag( "scale" ) ).add( "1 1 1" );
        xml::tag &instance( node.add( xml::tag( "instance_geometry" ) ) );
        instance.attributes().add( xml::attribute( "url", "#" + geomId ) );
        xml::tag &instanceMaterial( instance.add( xml::tag( "bind_material" ) ).add( xml::tag( "technique_common" ) ).add( xml::tag( "instance_material" ) ) );
        instanceMaterial.attributes().add( xml::attribute( "symbol", "WHITE" ) );
        instanceMaterial.attributes().add( xml::attribute( "target", "#whiteMaterial" ) );
        }

    root.add( xml::tag( "scene" ) ).add( xml::tag( "instance_visual_scene" ) ).attributes().add( xml::attribute( "url", "#DefaultScene" ) );

    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
        xml::xmlString( root, xml::xmlString::NewLines | xml::xmlString::Tabs ).toString();
    }

geometry::geometry( xml::tag &in ) : base( &(in.findFirst( "mesh" )) )
    {
    xml::tagList sources = base->findAll( "source" );

    jAssert( sources.size() );

    for( unsigned int x=0; x<sources.size(); x++ )
        {
        string name = sources[x].attributes().find( "id" ).toString();
        xml::tag &accessor = sources[x].findFirst( "technique_common" ).findFirst( "accessor" );
        jAssert( accessor.exists() );

        unsigned int size = accessor.findAll( "param" ).size();
        jAssert( size );

        xml::tag floatArray = sources[x].findFirst( "float_array" );
        jAssert( floatArray.exists() );
        unsigned int count = floatArray.attributes().find( "count" ).toUnsignedInt();


        switch( size )
            {
            case 2:
                {
                jAssert( ( count % 2 ) == 0 );
                arrayType <math::vector2> array;
                array.name = name;
                array.array = floatArray.contentArrayAs<math::vector2>( count / 2 );
                twoDimensionalArrays.push_back( array );
                }
            break;
            case 3:
                {
                jAssert( ( count % 3 ) == 0 );
                arrayType <math::triple> array;
                array.name = name;
                array.array = floatArray.contentArrayAs<math::triple>( count / 3 );
                threeDimensionalArrays.push_back( array );
                }
            break;
            case 4:
                {
                jAssert( ( count % 4 ) == 0 );
                arrayType <math::colour4> array;
                array.name = name;
                array.array = floatArray.contentArrayAs<math::colour4>( count / 4 );
                fourDimensionalArrays.push_back( array );
                }
            break;
            default:
                jFail( "Unsupported float array type" );
            break;
            }
        }
    }

unsigned int geometry::sizeTriangles()
    {
    return base->findAll( "triangles" ).size();
    }

unsigned int geometry::sizePolygons()
    {
    return  base->findAll( "polygons" ).size();
    }

unsigned int geometry::size()
    {
    return sizeTriangles() + sizePolygons();
    }

void geometry::get( unsigned int x, primitives::polygonMesh &mesh )
    {
    jAssert( x < size() );
    unsigned int inputNum = 0;
    unsigned int maxOffset = 0;
    int posOff = -1;
    int norOff = -1;
    int texOff = -1;

    xml::tagList inputs;
    if( x < sizeTriangles() )
        {
        inputs = base->findAll( "triangles" )[x].findAll( "input" );
        }
    else
        {
        inputs = base->findAll( "polygons" )[x-sizeTriangles()].findAll( "input" );
        }
    for( unsigned int in=0; in<inputs.size(); in++ )
        {
        xml::attributeList inputAttrs = inputs[in].attributes();
        string semantic = inputAttrs.find("semantic").toString();
        unsigned int offset = inputAttrs.find("offset").toUnsignedInt();
        string location;

        if( offset > maxOffset )
            {
            maxOffset = offset;
            }

        if( semantic == "VERTEX" )
            {
            xml::attributeList pos = base->findFirst( "vertices" ).findFirst( "input" ).attributes();
            jAssert( pos.find( "semantic" ).toString() == "POSITION" );
            semantic = "POSITION";
            location = pos.find("source").toString();
            }
        else
            {
            location = inputAttrs.find("source").toString();
            }

        location = location.substr(1);

        if( semantic == "POSITION" )
            {
            FOREACH( threeDimensionalArrays, array )
                {
                if( array->name == location )
                    {
                    mesh.points.set( array->array );
                    posOff = offset;
                    }
                }
            }
        if( semantic == "NORMAL" )
            {
            FOREACH( threeDimensionalArrays, array )
                {
                if( array->name == location )
                    {
                    mesh.normals.set( array->array );
                    norOff = offset;
                    }
                }
            }
        if( semantic == "TEXCOORD" )
            {
            FOREACH( twoDimensionalArrays, array )
                {
                if( array->name == location )
                    {
                    mesh.textures.set( array->array );
                    texOff = offset;
                    }
                }
            }

        ++inputNum;
        }

    jAssert( posOff != -1 );
    jAssert( norOff != -1 );
    jAssert( texOff != -1 );

    mesh.colours.resize( 1 );
    mesh.colours.add( math::colour4::WHITE );

    if( x < sizeTriangles() )
        {
        getTriangles( x, mesh, maxOffset, posOff, norOff, texOff );
        }
    else
        {
        getPolygons( x - sizeTriangles(), mesh, maxOffset, posOff, norOff, texOff );
        }
    }

void geometry::getPolygons( unsigned int x, primitives::polygonMesh &mesh, unsigned int maxOffset, int posOff, int norOff, int texOff )
    {
    xml::tag polygons = base->findAll( "polygons" )[x];
    unsigned int vertexSize = maxOffset + 1;

    xml::tagList ps = polygons.findAll( "p" );

    mesh.resizePolygonArray( ps.size() );

    unsigned int lastSize = 0;
    unsigned int polyNum = 0;
    for( unsigned int poly=0; poly<ps.size(); poly++ )
        {
        vector <unsigned int> indexes = ps[poly].contentArrayAs<unsigned int>( lastSize );
        lastSize = indexes.size();

        mesh.resizePolygon( polyNum, indexes.size() / vertexSize );
        for( unsigned int x=0; x<indexes.size(); x+=vertexSize )
            {
            unsigned int vertexID = x / vertexSize;
            mesh.setPolygonVertexPoint( polyNum, vertexID, indexes[x+posOff] );
            mesh.setPolygonVertexNormal( polyNum, vertexID, indexes[x+norOff] );
            mesh.setPolygonVertexTexture( polyNum, vertexID, indexes[x+texOff] );
            mesh.setPolygonVertexColour( polyNum, vertexID, 0 );
            }
        polyNum++;
        }
    mesh.rebuild();
    }

void geometry::getTriangles( unsigned int x, primitives::polygonMesh &mesh, unsigned int maxOffset, int posOff, int norOff, int texOff )
    {
    xml::tag triangles = base->findAll( "triangles" )[x];
    unsigned int vertexSize = maxOffset + 1;
    unsigned int triangleSize = 3 * vertexSize;

    vector <unsigned int> indexes = triangles.findFirst("p").contentArrayAs<unsigned int>( triangles.attributes().find("count").toUnsignedInt()*triangleSize );
    jAssert( ( indexes.size()%triangleSize ) == 0 );

    mesh.resizePolygonArray( indexes.size() / triangleSize );

    unsigned int triNum = 0;
    for( unsigned int x=0; x<indexes.size(); x+=triangleSize )
        {
        mesh.resizePolygon( triNum, 3 );
        for( unsigned int y=0; y<3; y++ )
            {
            mesh.setPolygonVertexPoint( triNum, y, indexes[x+(y*vertexSize)+posOff] );
            mesh.setPolygonVertexNormal( triNum, y, indexes[x+(y*vertexSize)+norOff] );
            mesh.setPolygonVertexTexture( triNum, y, indexes[x+(y*vertexSize)+texOff] );
            mesh.setPolygonVertexColour( triNum, y, 0 );
            }
        triNum++;
        }
    mesh.rebuild();
    }

END_JLIB_GRAPHICS_COLLADA_NAMESPACE
