#ifndef JLIB_GRAPHICS_PRIMITIVES_POLYGONMESH_H
#define JLIB_GRAPHICS_PRIMITIVES_POLYGONMESH_H

#include "graphics/primitives/graphics.primitives.types.h"
#include "graphics/primitives/graphics.primitives.triangleCloud.h"
#include "graphics/primitives/graphics.primitives.polygonMeshDataCache.h"

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE


class JLIB_EXPORT polygonMesh
    {
public:
    class polygonMeshPointCache : public polygonMeshDataCache <math::triple>
        {
    public:
        polygonMeshPointCache( vector <struct triangleArrayPointPrim> *slaveData ) :
                polygonMeshDataCache <math::triple> ( slaveData )
            {
            }
    protected:
        virtual void informSlave( unsigned int id )
            {
            math::triple point = _data[id].data;
            unsigned int *slaveChildrenIDs = &(_data[id].slaves[0]);
            unsigned int numSlaveChildren = _data[id].slaves.size();
            triangleArrayPointPrim *slaveChildren = &((*_slaveData)[0]);
            for( unsigned int x=0; x<numSlaveChildren; x++ )
                {
                slaveChildren[slaveChildrenIDs[x]].position.x = point.x();
                slaveChildren[slaveChildrenIDs[x]].position.y = point.y();
                slaveChildren[slaveChildrenIDs[x]].position.z = point.z();
                }
            }
        friend class polygonMesh;
        };
    class polygonMeshNormalCache : public polygonMeshDataCache <math::triple>
        {
    public:
        polygonMeshNormalCache( vector <struct triangleArrayPointPrim> *slaveData ) :
                polygonMeshDataCache <math::triple> ( slaveData )
            {
            }
    protected:
        virtual void informSlave( unsigned int id )
            {
            math::triple normal = _data[id].data;
            unsigned int *slaveChildrenIDs = &(_data[id].slaves[0]);
            unsigned int numSlaveChildren = _data[id].slaves.size();
            triangleArrayPointPrim *slaveChildren = &((*_slaveData)[0]);
            for( unsigned int x=0; x<numSlaveChildren; x++ )
                {
                slaveChildren[slaveChildrenIDs[x]].normal.x = normal.x();
                slaveChildren[slaveChildrenIDs[x]].normal.y = normal.y();
                slaveChildren[slaveChildrenIDs[x]].normal.z = normal.z();
                }
            }
        friend class polygonMesh;
        };
    class polygonMeshTextureCache : public polygonMeshDataCache <math::vector2>
        {
    public:
        polygonMeshTextureCache( vector <struct triangleArrayPointPrim> *slaveData ) :
                polygonMeshDataCache <math::vector2> ( slaveData )
            {
            }
    protected:
        virtual void informSlave( unsigned int id )
            {
            math::vector2 texture = _data[id].data;
            unsigned int *slaveChildrenIDs = &(_data[id].slaves[0]);
            unsigned int numSlaveChildren = _data[id].slaves.size();
            triangleArrayPointPrim *slaveChildren = &((*_slaveData)[0]);
            for( unsigned int x=0; x<numSlaveChildren; x++ )
                {
                slaveChildren[slaveChildrenIDs[x]].texture.x = texture.x();
                slaveChildren[slaveChildrenIDs[x]].texture.y = texture.y();
                }
            }
        friend class polygonMesh;
        };
    class polygonMeshColourCache : public polygonMeshDataCache <math::colour4>
        {
    public:
        polygonMeshColourCache( vector <struct triangleArrayPointPrim> *slaveData ) :
                polygonMeshDataCache <math::colour4> ( slaveData )
            {
            }
    protected:
        virtual void informSlave( unsigned int id )
            {
            math::colour4 colour = _data[id].data;
            unsigned int *slaveChildrenIDs = &(_data[id].slaves[0]);
            unsigned int numSlaveChildren = _data[id].slaves.size();
            triangleArrayPointPrim *slaveChildren = &((*_slaveData)[0]);
            for( unsigned int x=0; x<numSlaveChildren; x++ )
                {
                slaveChildren[slaveChildrenIDs[x]].colour.r = colour.r();
                slaveChildren[slaveChildrenIDs[x]].colour.g = colour.g();
                slaveChildren[slaveChildrenIDs[x]].colour.b = colour.b();
                slaveChildren[slaveChildrenIDs[x]].colour.a = colour.a();
                }
            }
        friend class polygonMesh;
        };
    //! Indexed elements
    polygonMeshPointCache points;
    polygonMeshNormalCache normals;
    polygonMeshTextureCache textures;
    polygonMeshColourCache colours;

    polygonMesh( triangleArray *slave, bool destroySlave=TRUE );
    ~polygonMesh( );

    //! Create and edit the polygon array
    void resizePolygonArray( unsigned int );
    unsigned int sizePolygonArray( ) const;
    void clearPolygonArray( );

    //! Edit or query a polygon
    void clearPolygon( unsigned int );
    void resizePolygon( unsigned int, unsigned int );
    unsigned int sizePolygon( unsigned int ) const;

    //! Control which indexed elements make up verticies
    unsigned int getPolygonVertexPoint( unsigned int, unsigned int ) const;
    unsigned int getPolygonVertexNormal( unsigned int, unsigned int ) const;
    unsigned int getPolygonVertexTexture( unsigned int, unsigned int ) const;
    unsigned int getPolygonVertexColour( unsigned int, unsigned int ) const;
    void setPolygonVertexPoint( unsigned int, unsigned int, unsigned int );
    void setPolygonVertexNormal( unsigned int, unsigned int, unsigned int );
    void setPolygonVertexTexture( unsigned int, unsigned int, unsigned int );
    void setPolygonVertexColour( unsigned int, unsigned int, unsigned int );

    //! Get or set the polygon rendering slave
    void rebuild();
    void setAll();
    triangleArray *getTriangleArray();
    const triangleArray *getTriangleArray() const;
    void setTriangleArray( triangleArray * );

    //! Polygon information
    jLib::list <jLib::pair<unsigned int, unsigned int> > listEdges() const;
    jLib::list <jLib::pair<unsigned int, unsigned int> > listTriangleEdges() const;

private:
    struct _vertexData
        {
        //int arrayID;
        unsigned int point;
        unsigned int normal;
        unsigned int texture;
        unsigned int colour;
        };
    struct _polygonData
        {
         vector <struct _vertexData> points;
         //unsigned int triangleStart;
         //unsigned int triangleNum;
        };
    triangleArray *_slave;
    bool _destroySlave;
    vector <struct _polygonData> _polygons;
    };

bool JLIB_EXPORT insertIfUnique( unsigned int a, unsigned int b, jLib::list <jLib::pair<unsigned int, unsigned int> > &li );

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
