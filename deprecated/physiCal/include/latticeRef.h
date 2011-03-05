#ifndef LATTICEREF_H
#define LATTICEREF_H

#include "physiCalGlobal.h"
#include "lattice.h"
#include "face.h"

BEGIN_PHYSICAL_NAMESPACE

class PHYSICAL_EXPORT latticeRef
    {
public:
    struct faceRef
        {
        position3 a;
        position3 b;
        position3 c;
        };
    latticeRef( const lattice & );

    const QList <position3> &particles() const;
    const QList <faceRef> &faces() const;

private:
    QList <position3> _particles;
    QList <faceRef> _faces;
    };

END_PHYSICAL_NAMESPACE

#endif
