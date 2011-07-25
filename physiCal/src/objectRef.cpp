#include "objectRef.h"

BEGIN_PHYSICAL_NAMESPACE

objectRef::objectRef( const object &obj )
    {
    // scope for foreach typedefs, get particles into my particle list
        {
        FOREACHCONST( obj.points(), p )
            {
            _particles.push_back( p->position() );
            }
        }
    // copy the springs into a my line list
        {
        FOREACHCONST( obj.springs(), s )
            {
            springRef temp;
            temp.a = s->primary().position();
            temp.b = s->secondary().position();
            _springs.push_back( temp );
            }
        }
    // copy the faces into my face list
        {
        FOREACHCONST( obj.faces(), f )
            {
            faceRef temp;
            temp.a = f->pointA();
            temp.b = f->pointB();
            temp.c = f->pointC();
            _faces.push_back(temp );
            }
        }
    }

const QList <position3> &objectRef::particles() const
    {
    return _particles;
    }

const QList <objectRef::springRef> &objectRef::springs() const
    {
    return _springs;
    }

const QList <objectRef::faceRef> &objectRef::faces() const
    {
    return _faces;
    }

END_PHYSICAL_NAMESPACE
