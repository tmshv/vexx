#include "frame.h"

BEGIN_PHYSICAL_NAMESPACE

frame::frame()
    {
    }

void frame::addObject( const object *in )
    {
    //Construct an objectRef and append to the list
    _lattices.push_back( objectRef( *in ) );
    }

void frame::addCollisions( const collisionArray &in )
    {
    // Add the collisions for debug drawing
    for( unsigned int x=0; x<in.size(); x++ )
        {
        // find the average face position, add a line off of the face
        line temp;
        temp.a = in[x].faces[0]->averagePosition();
        temp.b = temp.a + in[x].collisionPlane.normal.scaleTo( 0.1 );
        _collisions.push_back( temp );

        // repeat for other face
        temp.a = in[x].faces[1]->averagePosition();
        temp.b = temp.a + in[x].collisionPlane.normal.scaleTo( 0.1 );
        _collisions.push_back( temp );
        }
    }

void frame::addLog( const QString &in )
    {
    _log = in;
    }

const QList <objectRef> &frame::objects() const
    {
    return _lattices;
    }

const QList <frame::line> &frame::collisions() const
    {
    return _collisions;
    }

const QString &frame::log( ) const
    {
    return _log;
    }

END_PHYSICAL_NAMESPACE
