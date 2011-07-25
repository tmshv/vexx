#ifndef FRAME_H
#define FRAME_H

#include "physiCalGlobal.h"
#include "objectRef.h"

BEGIN_PHYSICAL_NAMESPACE

//! Class for holding a frame of cached simulation for display
class PHYSICAL_EXPORT frame
    {
public:
    //! A structure to hold a line for collisions
    struct line
        {
        position3 a;
        position3 b;
        };

    //! construct an empty frame
    frame();
    //! Add an object to the frame
    void addObject( const object * );
    //! Add an array of collisions to the array
    void addCollisions( const collisionArray & );
    //! Add a log entry to the array
    void addLog( const QString & );

    //! Return the list of objects in the frame
    const QList <objectRef> &objects() const;
    //! Return the list of collisions in the frame
    const QList <line> &collisions() const;
    //! Return the log for the frame
    const QString &log( ) const;

private:
    QList <objectRef> _lattices;
    QList <line> _collisions;
    QString _log;
    };

END_PHYSICAL_NAMESPACE

#endif
