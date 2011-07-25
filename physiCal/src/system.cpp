#include "system.h"
#include "QThread"
#include "timer.h"
#include "shape/shape.plane.h"

// comment to remove impulses from collisions ( probably a bad idea )
#define PHYSICAL_IMPULSE_COLLISIONS

// uncomment for self collisions ( slow idea )
//#define PHYSICAL_SELF_COLLISIONS

// multiply value for velocity on collision ( helps exploding significantly )
#define PHYSICAL_VELOCITY_DRAIN 0.990

BEGIN_PHYSICAL_NAMESPACE

// [internal] collide a point of a face with a plane.
inline void faceCollideBoth( pReal mult, particle *thP, objectFace *, plane cP, vector3 collisionNormal, bool impulseEnable, vector3 impulse )
    {
    // if the point is "inside" (behind) a plane
    if( ( mult * vector3( thP->position() - cP.position ).dot( cP.normal ) ) > 0 )
        {
        // a line from the point along the collision normal
        line aL( thP->position(), collisionNormal );
        // set it to the intersection position with the plane
        thP->setPosition( aL.sample( cP.collision( aL ) ) );

    #ifdef PHYSICAL_IMPULSE_COLLISIONS
        if( impulseEnable )
            {
            //set up velocity
            thP->setVelocity( ( thP->velocity() * PHYSICAL_VELOCITY_DRAIN ) + ( mult * impulse ) );
            }
    #endif
        }
    }
// [internal]
inline void faceCollideInfinite( particle *thP, objectFace *thF, plane cP, bool impulseEnable, vector3 impulse )
    {
    // collide the particle with an infinite surface
    faceCollideBoth( -1, thP, thF, cP, cP.normal, impulseEnable, impulse );
    }
// [internal]
inline void faceCollideFront( particle *thP, objectFace *thF, plane cP, bool impulseEnable, vector3 impulse )
    {
    // collide the front face with a particle
    faceCollideBoth( 1, thP, thF, cP, thF->normal(), impulseEnable, impulse );
    }
// [internal]
inline void faceCollideBack( particle *thP, objectFace *thF, plane cP, bool impulseEnable, vector3 impulse )
    {
    // collide the back face with a particle
    faceCollideBoth( -1, thP, thF, cP, thF->normal(), impulseEnable, impulse );
    }

// [internal] class for simulation threading
class simThread : public QThread
    {
    jProperty( system *, simulationSystem )

public:
    void run()
        {
        if( _simulationSystem )
            {
            // start the simulation
            _simulationSystem->simulate();
            }
        // exit (thread) on completion
        exit( 0 );
        }
    };

system::system() : _framesComplete( 0 )
    {
    // create a thread ( dont execute it )
    _currentThread = new simThread();

    // set up system log
    systemReport.assignHandler( FRAMELOG_ID, this );
    }

system::~system()
    {
    // delete thread
    delete _currentThread;
    // delete all objects
    FOREACH( _objects, o )
        {
        delete o.value();
        }
    }

void system::addMaterial( QString name, const springMaterial &in )
    {
    _materials.insert( name, springMaterial( in ) );
    }

void system::addObject( QString name, object *in )
    {
    _objects.insert( name, in );
    }

bool system::hasMaterial( QString in )
    {
    return _materials.contains( in );
    }

springMaterial &system::findMaterial( QString name )
    {
    // search for a material
    QHash <QString, springMaterial>::iterator it = _materials.find( name );
    if( it != _materials.end() )
        {
        return it.value();
        }
    jFail( "That material doesn't exist" );
    return *new springMaterial( 1 );
    }

object &system::findObject( QString name )
    {
    // search for an object
    QHash <QString, object *>::iterator it = _objects.find( name );
    if( it != _objects.end() )
        {
        return (*it.value());
        }
    jFail( "That lattice doesn't exist" );
    jLib::graphics::primitives::polygonMesh temp( 0 );
    return *new object( findMaterial( "NULL" ), temp );
    }

const QHash <QString, object *> &system::objects()
    {
    return _objects;
    }

void system::collide()
    {
    // setup a timer for job tracking
    timer::reset();
    pReal timeBegin = timer::time();

    // ensure bounding boxes are up to date
    FOREACH( _objects, l )
        {
        l.value()->refreshBoundingCube();
        }

    // empty collision array
    _collisions.clear();
    pReal timeSearchBegin = timer::time();
    QHash <QString, object *>::iterator l1, l2;
    // test objects against all other objects ( and maybe self, depending on PHYSICAL_SELF_COLLISIONS
    for( l1 = _objects.begin(); l1 != _objects.end(); l1++ )
        {
#ifdef PHYSICAL_SELF_COLLISIONS
        for( l2 = l1; l2 != _objects.end(); l2++ )
#else
        l2 = ++l1;
        --l1;
        for( ; l2 != _objects.end(); l2++ )
#endif
            {
            // add collisions to array
            l1.value()->collisions( l2.value(), _collisions );
            }
        }

    frameLog( _collisions.size()<<" collisions this frame." );

    // finished colliding, beginning resolving
    pReal timeResolveBegin = timer::time();

#ifndef PHYSICAL_IGNORE_COLLISIONS
    // resolve each collision
    for( unsigned int x=0; x<_collisions.size(); x++ )
        {
        plane cP = _collisions[x].collisionPlane;
        objectFace *singleFace = 0;

        // special values fro colliding with infinite surfaces
        if( _collisions[x].faces[0]->mass() == jLib::floatLimits::infinity() )
            {
            cP.normal = _collisions[x].faces[0]->normal();
            singleFace = _collisions[x].faces[1];
            }
        else if( _collisions[x].faces[1]->mass() == jLib::floatLimits::infinity() )
            {
            cP.normal = _collisions[x].faces[1]->normal();
            singleFace = _collisions[x].faces[0];
            }

        // calculate current separating velocity
        pReal sepV = _collisions[x].faces[0]->separatingVelocity( _collisions[x].collisionPlane.normal, *(_collisions[x].faces[1]) );
        // adjust to new separating velocity
        pReal newSepV = -sepV * _collisions[x].restitution;
        // the change in velocity ( different to rigid body systems )
        pReal deltaV = newSepV;
        // find impulse in direction of collision normal
        vector3 impulse = deltaV * _collisions[x].collisionPlane.normal;

        // if were hitting an infinite surface
        if( singleFace )
            {
            // need impulses? not if already separating
            bool impulseEnable( sepV > 0 );
            // collide each faces point
            faceCollideInfinite( singleFace->particleA(), singleFace, cP, impulseEnable, impulse );
            faceCollideInfinite( singleFace->particleB(), singleFace, cP, impulseEnable, impulse );
            faceCollideInfinite( singleFace->particleC(), singleFace, cP, impulseEnable, impulse );
            }
        else
            {
            // need impulses? not if already separating
            bool impulseEnable( sepV > 0 );
            // spread impulse over each face in ratio with masses
            impulse /= ( _collisions[x].faces[0]->mass() + _collisions[x].faces[1]->mass() );
            //Face 0
                {
                // collide each point with impulse relative to this faces mass
                vector3 impAdjusted( impulse * _collisions[x].faces[1]->mass() );
                faceCollideFront( _collisions[x].faces[0]->particleA(), _collisions[x].faces[0], cP, impulseEnable, impAdjusted );
                faceCollideFront( _collisions[x].faces[0]->particleB(), _collisions[x].faces[0], cP, impulseEnable, impAdjusted );
                faceCollideFront( _collisions[x].faces[0]->particleC(), _collisions[x].faces[0], cP, impulseEnable, impAdjusted );
                }
            //Face 1
                {
                // collide each point with impulse relative to this faces mass
                vector3 impAdjusted( impulse * _collisions[x].faces[0]->mass() );
                faceCollideBack( _collisions[x].faces[1]->particleA(), _collisions[x].faces[1], cP, impulseEnable, impAdjusted );
                faceCollideBack( _collisions[x].faces[1]->particleB(), _collisions[x].faces[1], cP, impulseEnable, impAdjusted );
                faceCollideBack( _collisions[x].faces[1]->particleC(), _collisions[x].faces[1], cP, impulseEnable, impAdjusted );
                }
            }
        }
#endif

    // print out time debug info
    pReal timeEnd = timer::time();

    pReal tTime = timeEnd - timeBegin;

    frameLog( "Total Time Colliding: "<<tTime );
    frameLog( "Percent Boundsing: "<<((timeSearchBegin-timeBegin)/tTime));
    frameLog( "Percent Searching: "<<((timeResolveBegin-timeSearchBegin)/tTime));
    frameLog( "Percent Resolving: "<<((timeEnd-timeResolveBegin)/tTime));
    }

void system::simulate()
    {
    _stop = FALSE;
    _frames.clear();
    _framesComplete = 0;

    // push back an initial state frame
    _frames.push_back( currentState() );
    _framesComplete = 1;

    // complete a step per frame requested
    for( unsigned int f=1; f<_totalFrames; f++ )
        {
        step();
        // break on request
        if( _stop )
            {
            break;
            }
        }
    }

void system::clearSimulation()
    {
    // end the simulation ( this function blocks while the current frame ends )
    endSimulation();
    _framesComplete = 0;
    // and clear frames if everything went well
    if( !isSimulating() )
        {
        _frames.clear();
        }
    }

void system::clearSetup()
    {
    // empty the system
    endSimulation();
    _objects.clear();
    _materials.clear();
    _collisions.clear();
    }

void system::step()
    {
    // clear the frame log
    _frameLog = "";
    frameLog( "Frame "<<_framesComplete<<" begin:" );

    // for each sub-step
    unsigned int num = (unsigned int) (_frameTime / _subFrameTime);
    for( unsigned int x=0; x<num; x++ )
        {
        // step each object
        FOREACH( _objects, l )
            {
            l.value()->integrate( _subFrameTime );
            }
        // collide all obejcts
        collide();
        }

    frameLog( "Frame "<<_framesComplete<<" end." );

    // append state to cache
    _frames.push_back( currentState() );

    _framesComplete += 1;
    }

void system::spawnSimulate()
    {
    // dont spawn if already spawned
    if( !(_currentThread->isRunning()) )
        {
        //setup thread
        _currentThread->simulationSystem( this );
        // start thread
        _currentThread->start();
        }
    }

bool system::isSimulating() const
    {
    return _currentThread->isRunning();
    }

void system::endSimulation()
    {
    // block while trying to stop simulation
    if( isSimulating() )
        {
        _stop = TRUE;
        while( isSimulating() ) { }
        }
    }

int system::handle( jLib::error::message in )
    {
    // append to frame log
    _frameLog += QString::fromStdString( in.message + "\n" );
    return jLib::error::RETURN;
    }

frame system::currentState() const
    {
    frame ret;

    // add collisions to start
    ret.addCollisions( _collisions );

    // add objects to state
    QHash <QString, object *>::const_iterator it = _objects.begin();
    for( ; it!= _objects.end(); it++ )
        {
        ret.addObject( it.value() );
        }

    // add log for debug
    ret.addLog( _frameLog );

    return ret;
    }

void system::getFrame( unsigned int x, frame &in ) const
    {
    // if we have the frame
    if( x < _framesComplete )
        {
        // return it
        in = _frames[x];
        }
    else if( _frames.size() )
        {
        // else return the back
        in = _frames.back();
        }
    }

END_PHYSICAL_NAMESPACE
