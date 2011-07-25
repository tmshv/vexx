#ifndef OBJECTLEAF_H
#define OBJECTLEAF_H

#include "physiCalGlobal.h"
#include "octree.h"
#include "objectFace.h"

BEGIN_PHYSICAL_NAMESPACE

//! An octree leaf of an object, used with the octree template
class objectLeaf
    {
public:
    //! create an octree leaf
    objectLeaf( octree <objectLeaf> * );
    //! setup this leaf with the faces given ( will also create children )
    void setup( QList <objectFace> & );
    //! refresh the bounding cube of this leaf ( and its children
    void refreshBoundingCube();

private:
    octree <objectLeaf> *_parent;
    boundingCube _bounds;
    QList <objectFace *> _faces;
    void setupBox( unsigned int, unsigned int, unsigned int );
    };

END_PHYSICAL_NAMESPACE

#endif
