#include "jlibpatchtest.h"

jLibPatchTest::jLibPatchTest( QWidget *parent ) : viewportWidgetBase( parent ),
        objectScene( jLib::math::colour4::BLACK ), transManip( ),
        sph( objectScene.passP() )
    {
    objectScene.pass().camera()->translate = jLib::math::triple( 10, 2, 10 );

    jLib::math::matrix4x4 mat = jLib::math::matrix4x4::IDENTITY;

    mat.setTranslation( jLib::math::triple( 1, 1, 1 ) );

    mat.setOrientation( jLib::math::quaternion::fromEulerAngles( jLib::math::triple( 45, 19, 99 ) ) );

    manipulators.setScene( objectScene );
    }

jLibPatchTest::~jLibPatchTest( )
    {
    }

void jLibPatchTest::setupGL()
    {
    int x=0;
    glutInit( &x, 0 );

    architecture.empty();
    architecture.updateContext();
    architecture.setFrameBufferFormat( jLib::graphics::render::frameBuffer::COLOUR_64BIT_FLOAT, jLib::graphics::render::frameBuffer::DEPTH_32BIT_FLOAT );
    architecture.addScene( "base", objectScene );

    architecture.addPostProcess( "ambOcc1", jLib::file::ioFile::get( "postFrag.txt" ) );
    architecture.addPostProcess( "ambOcc2", jLib::file::ioFile::get( "postFrag2.txt" ) );
    architecture.addPostProcess( "combine", jLib::file::ioFile::get( "combineFrag.txt" ) );

    architecture.setRenderable( "combine" );

    architecture.addPostProcessInput( "ambOcc1", "base", "ambOcc", jLib::graphics::render::architecture::DEPTH );

    architecture.addPostProcessInput( "ambOcc2", "ambOcc1", "depth", jLib::graphics::render::architecture::COLOUR );

    architecture.addPostProcessInput( "combine", "ambOcc2", "ambOcc", jLib::graphics::render::architecture::COLOUR );
    architecture.addPostProcessInput( "combine", "base", "colour", jLib::graphics::render::architecture::COLOUR );
    }

void jLibPatchTest::refresh()
    {
    static float angle = 0;

    angle += 0.1;
    float posA = sin( angle ) * 5;
    float posB = cos( angle ) * 5;

    jLib::graphics::shape::sphere sph( 0, 5 );
    jLib::graphics::shape::line ln( jLib::math::triple( posA, 0, 0 ), jLib::math::triple( 0.01, posB, posA ) );

    JFLOAT a, b;

    sph.collision( ln, a, b );

    redraw();
    }

SINGLEWIDGETAPP( jLibPatchTest )
