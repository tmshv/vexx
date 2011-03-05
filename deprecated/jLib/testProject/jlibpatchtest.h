#ifndef JLIBPATCHTEST_H
#define JLIBPATCHTEST_H

#define JLIB_DEBUG_RENDER

#include "jLib.h"
#include <QWidget>
#include "GL/glut.h"

class testScene : public jLib::graphics::entity::entityD
    {
    jLib::graphics::shader::lambert *shader;
    jLib::graphics::shader::light l1;
    jLib::graphics::shader::sLight light1Var;
    //jLib::graphics::shader::sMatRef4x4 cameraTx;
public:
    testScene( jLib::graphics::entity::entityG *p ) : entityD( p ), l1( p ), light1Var( 0 )//, cameraTx( "inverseCameraTransform" )
        {
        setDrawable();
        shader = 0;
        light1Var.set( l1 );
        //cameraTx.set( &(p->camera()->inverseview()) );
        }
    void draw()
        {
        if( !shader )
            {
            shader = new jLib::graphics::shader::lambert( );
            setShader( *shader );
            shaderVariableSet().add( light1Var );
            shaderVariableSet().setShader( *shader );
            setUseShaderVariableSet( );

            l1.ambient( jLib::math::triple( 0, 1, 0 ) );
            l1.diffuse( jLib::math::triple( 1, 0, 0 ) );
            l1.specular( jLib::math::triple( 0, 0, 1 ) );
            l1.direction( jLib::math::triple( 0, -1, 0 ) );
            l1.position( jLib::math::triple( 0, 5, 0 ) );
            l1.coneAngle( DEGTORAD * 15 );
            l1.linearAttenuation( 0 );
            l1.constantAttenuation( 1 );
            l1.quadraticAttenuation( 1 );
            }
        glColor3f( 1, 0, 0 );
        glPushMatrix();
            glTranslatef( 1.5, 1.45, 2 );
            glutSolidTeapot( 2 );
        glPopMatrix();

        glColor3f( 0, 1, 0 );
        glPushMatrix();
            glTranslatef( -2, 1, -2 );
            glutSolidSphere( 1, 30, 30 );
        glPopMatrix();

        glPushMatrix();
            glTranslatef( 2, 0.5, -2 );
            glPushMatrix();
                glColor3f( 0, 0, 1 );
                glRotatef( 36, 0, 1, 0 );
                glTranslatef( -0.5, 0, -0.6 );
                glutSolidCube( 1 );
            glPopMatrix();
            glPushMatrix();
                glColor3f( 1, 1, 0 );
                glRotatef( -4, 0, 1, 0 );
                glTranslatef( 0.6, 0, -0.4 );
                glutSolidCube( 1 );
            glPopMatrix();
            glPushMatrix();
                glColor3f( 0, 1, 1 );
                glRotatef( 50, 0, 1, 0 );
                glTranslatef( 0, 1, -0.4 );
                glutSolidCube( 1 );
                glColor3f( 1, 0, 1 );
                glRotatef( 36, 0, 1, 0 );
                glTranslatef( 0, 1, 0.2 );
                glutSolidCube( 1 );
            glPopMatrix();
        glPopMatrix();

        glColor3f( 1, 1, 1 );
        glBegin( GL_QUADS );
            glNormal3f( 0, 1, 0 );
            glVertex3f( 5, 0, 5 );
            glVertex3f( 5, 0, -5 );
            glVertex3f( -5, 0, -5 );
            glVertex3f( -5, 0, 5 );
        glEnd();
        }
    };

class jLibPatchTest : public jLib::jqt::viewportWidgetBase
    {
    JLIB_QT_MANIPULATOR_WIDGET
public:
    jLibPatchTest( QWidget *parent = 0 );
    ~jLibPatchTest( );

    jLib::graphics::render::scene objectScene;

    /*jLib::graphics::shader::postProcess *pProcess;
    jLib::graphics::render::frameBuffer *fBuffer;

    jLib::graphics::shader::postProcess *pProcess2;
    jLib::graphics::render::frameBuffer *fBuffer2;
    jLib::graphics::render::scene ambOccScene;*/

    //jLib::graphics::implicit::grid grid;
    jLib::graphics::viewport::manipulators::translate transManip;
    jLib::graphics::shader::surfaceShader *shader;
    jLib::graphics::shader::variableSet set;
    //jLib::graphics::shader::sVec3 vec;
    testScene sph;

    void setupGL();
    void refresh();
    };

#endif
