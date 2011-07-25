#ifndef OBJECTREF_H
#define OBJECTREF_H

#include "physiCalGlobal.h"
#include "object.h"

BEGIN_PHYSICAL_NAMESPACE

//! Represents an object in a cached frame, we only store positions of the involved elements, not velocity and such
class PHYSICAL_EXPORT objectRef
    {
public:
    //! a structure to hold a face
    struct faceRef
        {
        position3 a;
        position3 b;
        position3 c;
        };
    //! a structure to hold a spring
    struct springRef
        {
        position3 a;
        position3 b;
        };

    //! create an objectRef from the object given
    objectRef( const object & );

    //! the particles associated with this reference
    const QList <position3> &particles() const;
    //! the springs associated with this reference
    const QList <springRef> &springs() const;
    //! the facess associated with this reference
    const QList <faceRef> &faces() const;

private:
    QList <position3> _particles;
    QList <springRef> _springs;
    QList <faceRef> _faces;
    };

END_PHYSICAL_NAMESPACE

#endif // OBJECTREF_H
