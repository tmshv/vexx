#include "latticeRef.h"

BEGIN_PHYSICAL_NAMESPACE

latticeRef::latticeRef( const lattice &in )
    {
        {
        FOREACHCONST( in.particles(), p )
            {
            _particles.push_back( p->position() );
            }
        }
        {
        FOREACHCONST( in.faces(), f )
            {
            faceRef temp;

            temp.a = f->pointA();
            temp.b = f->pointB();
            temp.c = f->pointC();

            _faces.push_back( temp );
            }
        }
    }

const QList <position3> &latticeRef::particles() const
    {
    return _particles;
    }

const QList <latticeRef::faceRef> &latticeRef::faces() const
    {
    return _faces;
    }

END_PHYSICAL_NAMESPACE
