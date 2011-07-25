#ifndef JLIB_GRAPHICS_PRIMITIVES_TRIANGLEARRAY_H
#define JLIB_GRAPHICS_PRIMITIVES_TRIANGLEARRAY_H

#include "graphics/primitives/graphics.primitives.types.h"
#include "graphics/entity/graphics.entity.h"
#include "graphics/shape/graphics.shape.cuboid.h"

#ifndef TRIANGLEARRAY_NEEDS_NORMAL
#   define TRIANGLEARRAY_NEEDS_NORMAL 1
#endif
#ifndef TRIANGLEARRAY_NEEDS_TEXTURE
#   define TRIANGLEARRAY_NEEDS_TEXTURE 1
#endif
#ifndef TRIANGLEARRAY_NEEDS_COLOUR
#   define TRIANGLEARRAY_NEEDS_COLOUR 1
#endif

BEGIN_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

struct triangleArrayIndexPrim
    {
    unsigned int a;
    unsigned int b;
    unsigned int c;
    };

struct triangleArrayPointPrim
    {
    math::quickPt3 position;
#if TRIANGLEARRAY_NEEDS_NORMAL
    math::quickPt3 normal;
#endif
#if TRIANGLEARRAY_NEEDS_TEXTURE
    math::quickPt2 texture;
#endif
#if TRIANGLEARRAY_NEEDS_COLOUR
    math::quickPt4 colour;
#endif
    };

class JLIB_EXPORT triangleArray : public entity::entityD
    {
public:
#if !TRIANGLEARRAY_NEEDS_COLOUR
    colour4 colour;
#endif
    bool alphaBlend;

    triangleArray( entity::entityG *p );
    ~triangleArray( );

    void setTriangle( unsigned int, unsigned int, unsigned int, unsigned int );
    void setTriangleA( unsigned int, unsigned int );
    void setTriangleB( unsigned int, unsigned int );
    void setTriangleC( unsigned int, unsigned int );
    unsigned int getTriangleA( unsigned int ) const;
    unsigned int getTriangleB( unsigned int ) const;
    unsigned int getTriangleC( unsigned int ) const;
    virtual void removeTriangle( unsigned int );
    void setTriangleArraySize( unsigned int );
    unsigned int sizeTriangles( ) const;
    virtual void clearTriangles( );
    void setPoint( unsigned int, math::triple, math::triple, math::vector2, math::colour4 );
    void setPointPosition( unsigned int, math::triple );
    void setPointNormal( unsigned int, math::triple );
    void setPointTexture( unsigned int, math::vector2 );
    void setPointColour( unsigned int, math::colour4 );
    math::triple getPointPosition( unsigned int ) const;
    math::triple getPointNormal( unsigned int ) const;
    math::vector2 getPointTexture( unsigned int ) const;
    math::colour4 getPointColour( unsigned int ) const;
    virtual void removePoint( unsigned int );
    void setPointArraySize( unsigned int );
    unsigned int sizePoints( ) const;
    virtual void clearPoints( );
    virtual void clear( );
    virtual void draw( );

    jLib::graphics::shape::cuboid bounds() const;

protected:
    vector <struct triangleArrayIndexPrim> _indexData;
    vector <struct triangleArrayPointPrim> _pointData;

    friend class polygonMesh;
    };

/*class triangleCloud : public entityD
    {
public:
#if !TRIANGLECLOUD_NEEDS_COLOUR
    colour4 colour;
#endif
    bool alphaBlend;

    triangleCloud( entityG *p );
    ~triangleCloud( );

    unsigned int addPoint( math::point4, math::vector4 normal=math::vector4( 0 ), math::vector2=math::vector2( 0 ), colour4 c=colour4(1) );
    unsigned int addPoint( math::point4, math::vector2=math::vector2( 0 ), colour4 c=colour4(1) );
    unsigned int addPoint( math::point4, colour4 c );
    void removePoint( unsigned int );
    unsigned int addTriangle( unsigned int, unsigned int, unsigned int );
    unsigned int addTriangles( unsigned int *, size_t );
    unsigned int addQuad( unsigned int, unsigned int, unsigned int, unsigned int );
    unsigned int addQuads( unsigned int *, size_t );
    unsigned int addPolygon( unsigned int *, size_t );
    unsigned int addPolygon( vector<unsigned int> );
    void removeTriangle( unsigned int );
    unsigned int numTriangles();
    unsigned int numPoints();

    virtual void draw( );

protected:
    unsigned int _numIndexs;
    unsigned int _numPoints;
    unsigned int *_indexBuf;
    struct triangleCloudPointPrim *_vertexBuf;
    };*/

END_JLIB_GRAPHICS_PRIMITIVES_NAMESPACE

#endif
