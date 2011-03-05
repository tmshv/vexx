#include "physicalAlterSimulation.h"
#include "objectEntity.h"
#include "materialEntity.h"
#include "groundObject.h"
#include "freezeBox.h"
#include "viewportDock.h"
#include "ALog"

#include "QFile"
#include "QDebug"

physicalAlterSimulation::physicalAlterSimulation() : _group( 0 ), _point( &_group ), _line( &_group ), _tri( &_group )
    {
    // create the physical system
    _system = new cal::system();
    // set up my draw object
    object( &_group );
    // set up my draw styles
    _point.setDrawSize( 3 );
    // we create this shader when the context is active
    _shader = 0;
    }

physicalAlterSimulation::~physicalAlterSimulation()
    {
    // delete created objects
    delete _system;
    if( _shader )
        {
        delete _shader;
        }
    }

void physicalAlterSimulation::added()
    {
    // we need to monitor the array
    connect( app(), SIGNAL(entityAdded(AEntity*)), this, SLOT(arrayChanged(AEntity*)) );
    connect( app(), SIGNAL(entityRemoved(AEntity*)), this, SLOT(arrayChanged(AEntity*)) );

    // get a pointer to the viewport
    viewportDock *dock( app()->accessAs<viewportDock *>("basicViewport") );

    // add our draw object as a viewportTool
    dock->addTool( this );
    // get a context active signal
    connect( dock, SIGNAL(contextActivate()), this, SLOT(inititialiseGL()) );

    // add draw types
    dock->addDrawType( "PhysiCal Points" );
    dock->addDrawType( "PhysiCal Lines" );
    dock->addDrawType( "PhysiCal Meshes" );

    //connect for types signals
    connect( dock, SIGNAL(showTypesChanged()), this, SLOT(showTypesChanged()) );
    showTypesChanged();
    }

void physicalAlterSimulation::arrayChanged( AEntity * )
    {
    // enumerate all the physical objects in the scene
    AEntitySearch search( app()->search().findType( objectEntity::identifier() ) +
                                 app()->search().findType( materialEntity::identifier() ) +
                                 app()->search().findType( groundObject::identifier() ) +
                                 app()->search().findType( freezeBox::identifier() ) );

    // connect to all their changed signals
    FOREACH( search, s )
        {
        connect( *s, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChanged(APropertyDevice*,AProperty*)), Qt::UniqueConnection );
        }

    // and trigger the setup changed
    if( search.size() )
        {
        setupChanged();
        }
    }

void physicalAlterSimulation::showTypesChanged()
    {
    // dont draw unwanted things
    viewportDock *dock( app()->accessAs<viewportDock *>("basicViewport") );
    drawPoints = dock->shouldDraw( "PhysiCal Points" );
    drawLines = dock->shouldDraw( "PhysiCal Lines" );
    drawTris = dock->shouldDraw( "PhysiCal Meshes" );
    update();
    }

void physicalAlterSimulation::entityChanged( APropertyDevice *, AProperty * )
    {
    // trigger system reset
    setupChanged();
    }

void physicalAlterSimulation::setupSimulate()
    {
    //Global Infinite material
    _system->addMaterial( "infinite", cal::springMaterial::infinite() );

    //Add Materials
        {
        // search for materials
        AEntitySearch search( app()->search().findType( materialEntity::identifier() ) );

        // add them to the system
        FOREACH( search, mat )
            {
            _system->addMaterial( (*mat)->name(),
                cal::springMaterial( (*mat)->property("density")->toDouble(),
                                     (*mat)->property("restitution")->toDouble(),
                                     (*mat)->property("springConstant")->toDouble(),
                                     (*mat)->property( "springDamping" )->toDouble(),
                                     (*mat)->property("elastic")->toDouble(),
                                     (*mat)->property("plastic")->toDouble() ) );
            }
        }

    //Add Objects
        {
        // search for objects
        AEntitySearch search( app()->search().findType( objectEntity::identifier() ) );

        //add each
        FOREACH( search, object )
            {
            // if the material is valid
            if( _system->hasMaterial( (*object)->property( "material" )->toString() ) &&
                (*object)->castTo<objectEntity *>()->mesh().sizePolygonArray() > 0 )
                {
                // add the object to the system
                _system->addObject( (*object)->name(),
                    new cal::object( _system->findMaterial( (*object)->property( "material" )->toString() ),
                                    (*object)->castTo<objectEntity *>()->mesh(),
                                    cal::pVector( (*object)->property( "position" )->toVector3D() ),
                                    cal::pVector( (*object)->property( "velocity" )->toVector3D() ),
                                    (*object)->property( "randomPoints" )->toInt(),
                                    (*object)->property( "randomSprings" )->toInt(),
                                    (*object)->property( "maxLength" )->toDouble() ) );
                }
            else
                {
                log( "There was a problem adding " + (*object)->name() + " to the simulation, ensure it has geometry and a material" );
                }
            }
        }

    //Freeze Objects
        {
        // search for freeze boxes
        AEntitySearch search( app()->search().findType( freezeBox::identifier() ) );

        FOREACH( search, box )
            {
            // create a cuboid for testing points against
            cal::cuboid frBox( cal::pVector( (*box)->property( "minimum" )->toVector3D() ),
                               cal::pVector( (*box)->property( "maximum" )->toVector3D() ) );

            // freeze points inside the box
            FOREACHCONST( _system->objects(), o )
                {
                FOREACH( (*o)->points(), p )
                    {
                    if( frBox.test( p->position() ) )
                        {
                        p->mass( jLib::floatLimits::infinity() );
                        }
                    }
                }
            }
        }

    //Add Grounds
        {
        // add the ground objects
        AEntitySearch search( app()->search().findType( groundObject::identifier() ) );

        FOREACH( search, object )
            {
            /// create and add ground objects
            _system->addObject( (*object)->name(),
                    cal::object::ground(_system->findMaterial( "infinite" ),
                    (*object)->property( "xRes" )->toDouble(),
                    (*object)->property( "zRes" )->toDouble() ) );
            }
        }
    }

void physicalAlterSimulation::clearSimulationCache( )
    {
    // clear the underlying system
    _system->clearSimulation();
    }

void physicalAlterSimulation::setDisplayMode( displayMode in )
    {
    // find drawable types
    AEntitySearch search( app()->search().findType( objectEntity::identifier() ) +
                                 app()->search().findType( groundObject::identifier() ) );

    // hide objects during cache display ( we draw the cached results )
    if( in == abstractSimulator::Setup )
        {
        _point.clear();
        _line.clear();
        _tri.clear();
        FOREACH( search, s )
            {
            (*s)->castTo<viewportEntity *>()->visible( TRUE );
            }
        }
    else
        {
        FOREACH( search, s )
            {
            (*s)->castTo<viewportEntity *>()->visible( FALSE );
            }
        }
    }

void physicalAlterSimulation::setDisplayTime( JFLOAT now )
    {
    // update the system
    _system->getFrame( now / _system->frameTime(), _currentFrame );
    update();
    }

void physicalAlterSimulation::update()
    {
    // update the draw caches
    _point.clear();
    _line.clear();
    _tri.clear();

    jLib::math::triple n( 0.0 );
    jLib::math::vector2 v( 0.0 );

    float hue=0;
    float adder = JM2xPI / _currentFrame.objects().size();
    FOREACHCONST( _currentFrame.objects(), l )
        {
        //Particles
        if( drawPoints )
            {
            FOREACHCONST( l->particles(), p )
                {
                _point.addPoint( *p, jLib::math::colour4::YELLOW );
                }
            }
        //Springs
        if( drawLines )
            {
            FOREACHCONST( l->springs(), s )
                {
                _line.addLine( _line.addPoint( s->a, jLib::math::colour4::WHITE ),
                               _line.addPoint( s->b, jLib::math::colour4::WHITE ) );
                }
            }
        //Triangles
        if( drawTris )
            {
            jLib::math::colour4 c( cos( hue ), cos( hue + DEGTORAD*120 ), cos( hue + DEGTORAD*240 ) );
            FOREACHCONST( l->faces(), f )
                {
                n = ( f->b - f->a ).cross( ( f->c - f->a ) ).normalise();
                _tri.addTriangle( _tri.addPoint( f->a, n, v, c ),
                                  _tri.addPoint( f->b, n, v, c ),
                                  _tri.addPoint( f->c, n, v, c ) );
                }
            }
        hue += adder;
        }
    }

void physicalAlterSimulation::setStepTime( JFLOAT in )
    {
    // setup system times
    _system->frameTime( in );
    _system->subFrameTime( 0.0002 );
    }

void physicalAlterSimulation::simulateStep()
    {
    _system->step();
    }

JFLOAT physicalAlterSimulation::maximumAvailableTime() const
    {
    return _system->framesComplete() * _system->frameTime();
    }

QString physicalAlterSimulation::currentTimeLogText( ) const
    {
    return _currentFrame.log();
    }

void physicalAlterSimulation::inititialiseGL()
    {
    // this signal is emitted while the context is active for setting the system up

    viewportDock *dock( app()->accessAs<viewportDock *>("basicViewport") );
    disconnect( dock, SIGNAL(contextActivate()), this, SLOT(inititialiseGL()) );

    // load the cache shader
    QFile vert(":/simulationVert.txt"), frag( ":/simulationFrag.txt" );
    if ( !vert.open( QIODevice::ReadOnly | QIODevice::Text ) || !frag.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
        return;
        }

    _shader = new jLib::graphics::shader::surfaceShader( QString( frag.readAll() ).toStdString(),
                                                         QString( vert.readAll() ).toStdString() );

    _tri.setShader( *_shader );
    }
