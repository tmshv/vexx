#ifndef GRAPHICS_COLLADA_GEOMETRY_H
#define GRAPHICS_COLLADA_GEOMETRY_H

#include "graphics/collada/graphics.collada.types.h"
#include "graphics/primitives/graphics.primitives.polygonMesh.h"

BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE

class JLIB_EXPORT geometry
    {
public:
    geometry( xml::tag & );

    unsigned int size();
    void get( unsigned int, primitives::polygonMesh & );

private:
    unsigned int sizeTriangles();
    unsigned int sizePolygons();
    void getTriangles( unsigned int, primitives::polygonMesh &, unsigned int, int, int, int );
    void getPolygons( unsigned int, primitives::polygonMesh &, unsigned int, int, int, int );
    template <typename T> struct arrayType
        {
        string name;
        vector <T> array;
        };
    vector <arrayType <math::vector2> > twoDimensionalArrays;
    vector <arrayType <math::triple> > threeDimensionalArrays;
    vector <arrayType <math::colour4> > fourDimensionalArrays;
    xml::tag *base;
    };

typedef vector <geometry> geometryArray;

string meshToCollada( const primitives::polygonMesh & );

END_JLIB_GRAPHICS_COLLADA_NAMESPACE

#endif
