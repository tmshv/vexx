#ifndef SYSTEM_H
#define SYSTEM_H

#include "physiCalGlobal.h"
#include "object.h"
#include "frame.h"
#include "frameLog.h"
#include "QAtomicInt"

BEGIN_PHYSICAL_NAMESPACE

#define PHYSICAL_COLLISION_GIVE 0.00

class simThread;

//! Holds a system, effectively a group of object classes, which are simulated in the same world.
class PHYSICAL_EXPORT system :
        public jLib::error::reportHandler
    {
public:
    //! holds the number of frames this simulation will complete
    jProperty( unsigned int, totalFrames );
    //! holds the time a sub frame lasts
    jProperty( pReal, subFrameTime );
    //! holds the time a frame lasts
    jProperty( pReal, frameTime );
    //! holds the number of frames this simulation has completed
    jROProperty( unsigned int, framesComplete );

public:
    //! create an empty system
    system();
    //! destroy this system and all contained objects
    virtual ~system();
    //! add a material to the system
    void addMaterial( QString, const springMaterial & );
    //! add an object to the system
    void addObject( QString, object * );
    //! find a material in the system
    springMaterial &findMaterial( QString );
    //! find if a material exists
    bool hasMaterial( QString );
    //! find an object in the system
    object &findObject( QString );
    //! begin simulating ( blocks until complete - see spawnSimulate )
    void simulate();
    //! step one frame forward in the simulation ( blocks )
    void step();
    //! find the current state of the system in a frame object
    frame currentState() const;
    //! clear the simulation data ( cached frames )
    void clearSimulation();
    //! clear the simulation setup
    void clearSetup();

    //! find the contained objects
    const QHash <QString, object *> &objects();

    //! get the frame at frame t into f
    void getFrame( unsigned int t, frame &f ) const;

    //! spawn a simulation thread
    void spawnSimulate();
    //! find if the simulation thread is active
    bool isSimulating() const;
    //! end the simulation thread ( blocks until complete )
    void endSimulation();

private:
    virtual int handle( jLib::error::message );

    void collide();
    QList <frame> _frames;
    QHash <QString, object *> _objects;
    simThread *_currentThread;
    QHash <QString, springMaterial> _materials;
    bool _stop;
    int pause;
    QString _frameLog;
    collisionArray _collisions;
    QAtomicInt _shouldStop;
    };

END_PHYSICAL_NAMESPACE

#endif
