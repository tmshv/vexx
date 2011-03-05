#include "main.h"
#include "QKeyEvent"

using namespace jLib::math;

physiCalTest::physiCalTest( QWidget *parent ) : viewportWidgetBase( parent ), objectScene( colour4::BLACK ),
        lines( objectScene.passP() ), points( objectScene.passP() ), tris( objectScene.passP() ),
        play( Stop ), drawPoints( TRUE ), drawLines( FALSE ), drawTriangles( TRUE ), drawDebug( FALSE ),
        drawRefresh( FALSE ), frameRequest( 0 )
    {
    manipulators.setScene( objectScene );

    points.setDrawSize( 5 );

    objectScene.camera().translate = jLib::math::triple( 10, 2, 10 );

    tris.alphaBlend = TRUE;


    jLib::graphics::primitives::polygonMesh polyMesh( new jLib::graphics::primitives::triangleArray( objectScene.passP() ) );
    jLib::graphics::collada::file collada( jLib::file::ioFile( "sphere.dae" ) );
    jLib::vector <jLib::graphics::collada::geometry> geos = collada.get<jLib::graphics::collada::geometry>( jLib::graphics::collada::base::geometry );

    /*jLib::graphics::primitives::polygonMesh polyMesh2( new jLib::graphics::primitives::triangleArray( objectScene.passP() ) );
    jLib::graphics::collada::file collada2( jLib::file::ioFile( "bar.dae" ) );
    jLib::vector <jLib::graphics::collada::geometry> geos2 = collada2.get<jLib::graphics::collada::geometry>( jLib::graphics::collada::base::geometry );

    jLib::graphics::primitives::polygonMesh polyMesh3( new jLib::graphics::primitives::triangleArray( objectScene.passP() ) );
    jLib::graphics::collada::file collada3( jLib::file::ioFile( "sphereLow.dae" ) );
    jLib::vector <jLib::graphics::collada::geometry> geos3 = collada3.get<jLib::graphics::collada::geometry>( jLib::graphics::collada::base::geometry );
*/

    geos[0].get( 0, polyMesh );
    //geos2[0].get( 0, polyMesh2 );
    //geos3[0].get( 0, polyMesh3 );

    sys.addMaterial( "ground", cal::springMaterial::infinite() );

    sys.addMaterial( "Steel", cal::springMaterial( 1, 0.3, 750, 0.10 ) );
    //sys.addMaterial( "Jelly", cal::springMaterial( 10, 1, 1000, 1000, 1000, 0.2 ) );

    sys.addObject( "groundPlane", cal::object::ground(  sys.findMaterial( "ground" ), 15, 15 ) );

    sys.addObject( "box1", new cal::object( sys.findMaterial( "Steel" ), polyMesh, cal::position3( 0, 5, 0 ), cal::vector3( 0, -20, 0 ), 100, 3  ) );
    //sys.addObject( "box2", new cal::object( sys.findMaterial( "Steel" ), polyMesh3, cal::position3( 0, 1.5, 0 ), cal::vector3( 0, 0, 0 ), 50, 2  ) );

    //sys.addObject( "box2", new cal::object( sys.findMaterial( "Steel" ), polyMesh3, cal::position3( 0.8, 3, 0.8 ), cal::vector3( 0, 0, 0 ), 0, 2  ) );
    //sys.addObject( "box3", new cal::object( sys.findMaterial( "Steel" ), polyMesh3, cal::position3( 0, 10, 0 ), cal::vector3( 0, 0, 0 ), 0, 2  ) );
    //sys.addObject( "box2", new cal::object( sys.findMaterial( "Steel" ), polyMesh, cal::position3( 0, 4, 0 ), cal::vector3( 0, 0, 0 ), 0, 4 ) );
    //sys.addObject( "box2", new cal::object( sys.findMaterial( "Steel" ), polyMesh, cal::position3( 0, 2.6, 0 ), cal::vector3( 0, 0, 0 ), 14, 3 ) );
    //sys.addObject( "box2", new cal::object( sys.findMaterial( "Steel" ), polyMesh2, cal::position3( 4, 2.7, 4 ), cal::vector3( 0, 0, 0 ), 10, 4 ) );
    //sys.addObject( "box3", new cal::object( sys.findMaterial( "Steel" ), polyMesh2, cal::position3( 0, 15, 0 ), cal::vector3( 0, -30, 0 ), 5, 3 ) );
    //sys.addObject( "box4", new cal::object( sys.findMaterial( "Steel" ), polyMesh2, cal::position3( 0, 4, 0 ), cal::vector3( 0, 0, 0 ), 5, 3 ) );

    sys.frameTime( 0.04 );
    sys.subFrameTime( 0.0002 );
    sys.totalFrames( 20000 );
    sys.spawnSimulate();
    }

physiCalTest::~physiCalTest( )
    {
    if( sys.isSimulating() )
        {
        sys.endSimulation();
        }
    }

void physiCalTest::setupGL()
    {
    architecture.addScene( "base", objectScene );
    architecture.addPostProcess( "ambOcc", jLib::file::ioFile::get( "postFrag1.txt" ) );
    architecture.addPostProcess( "ambOccBlur", jLib::file::ioFile::get( "postFrag2.txt" ) );
    architecture.addPostProcess( "finalComp", jLib::file::ioFile::get( "postFrag3.txt" ) );

    architecture.addPostProcessInput( "ambOcc", "base", "depth", jLib::graphics::render::architecture::DEPTH );
    architecture.addPostProcessInput( "ambOccBlur", "ambOcc", "ambOcc", jLib::graphics::render::architecture::COLOUR );
    architecture.addPostProcessInput( "finalComp", "ambOccBlur", "ambOcc", jLib::graphics::render::architecture::COLOUR );
    architecture.addPostProcessInput( "finalComp", "base", "colour", jLib::graphics::render::architecture::COLOUR );
    architecture.setRenderable( "finalComp" );
    }

void physiCalTest::refresh()
    {
    if( play == Play )
        {
        drawRefresh = TRUE;
        if( direction == Forwards )
            {
            frameRequest++;
            if( frameRequest == sys.framesComplete() )
                {
                frameRequest = 0;
                }
            }
        else if( direction == Backwards )
            {
            if( frameRequest == 0 )
                {
                frameRequest = sys.framesComplete() - 1;
                }
            else
                {
                frameRequest--;
                }
            }
        }

    static unsigned int oldfComp = sys.framesComplete();
    if( oldfComp != sys.framesComplete() )
        {
        //jDebugNeat<<sys.framesComplete()<<" frames complete."<<endl;
        if( play == Auto )
            {
            frameRequest = ( sys.framesComplete() - 1 );
            drawRefresh = TRUE;
            }
        oldfComp = sys.framesComplete();
        }

    if( drawRefresh && frameRequest < sys.framesComplete() )
        {
        points.clear();
        tris.clear();
        lines.clear();

        cal::frame currentFrame;
        sys.getFrame( frameRequest, currentFrame );

        if( drawPoints )
            {
            FOREACHCONST( currentFrame.objects(), l )
                {
                FOREACHCONST( l->particles(), p )
                    {
                    points.addPoint( *p, colour4::YELLOW );
                    }
                }
            }


        if( drawLines )
            {
            FOREACHCONST( currentFrame.collisions(), c )
                {
                lines.addLine( c->a, c->b );
                }
            }
        else
            {
            FOREACHCONST( currentFrame.objects(), l )
                {
                {
                FOREACHCONST( l->springs(), p )
                    {
                    lines.addLine( p->a, p->b );
                    }
                }

                }
            }

        if( drawTriangles )
            {
            jLib::math::triple n( 0.0 );
            jLib::math::vector2 v( 0.0 );

            float hue=0;
            float adder = JM2xPI / currentFrame.objects().size();
            FOREACHCONST( currentFrame.objects(), l )
                {
                jLib::math::colour4 c( cos( hue ), cos( hue + DEGTORAD*120 ), cos( hue + DEGTORAD*240 ), 0.5 );
                FOREACHCONST( l->faces(), f )
                    {
                    tris.addTriangle( tris.addPoint( f->a, n, v, c ),
                                      tris.addPoint( f->b, n, v, c ),
                                      tris.addPoint( f->c, n, v, c ) );
                    //cal::vector3 norm( cal::vector3( f->b - f->a ).cross( f->c - f->a ).normalise() );
                    //cal::position3 avP( ( f->a + f->b + f->c ) / 3 );
                    //lines.addLine( avP, avP+norm );
                    }
                hue += adder;
                }
            }

        if( drawDebug )
            {
            jDebugNeat<<currentFrame.log()<<endl;
            }

        drawRefresh = FALSE;
        }

    redraw();
    }


SINGLEWIDGETAPP( physiCalTest )
