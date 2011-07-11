#include "GCScene.h"
#include "XModeller.h"
#include "XShader.h"
#include "XRenderer.h"
#include "GL/gl.h"
#include "XMatrix4x4"

S_IMPLEMENT_PROPERTY(GCScene)

SPropertyInformation *GCScene::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCScene>("GCScene");

  info->add(&GCScene::cameraTransform , "cameraTransform");
  info->add(&GCScene::cameraProjection , "cameraProjection");

  return info;
  }

GCScene::GCScene()
  {
  }

void GCScene::render(XRenderer *r) const
  {
  r->setProjectionTransform(cameraProjection());

    //glLoadIdentity();						// Reset The View


    /*glTranslatef(-1.5f,0.0f,-6.0f);					// Move Left 1.5 Units And Into The Screen 6.0


    glBegin(GL_TRIANGLES);						// Drawing Using Triangles
      glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
      glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
      glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
    glEnd();							// Finished Drawing The Triangle

    glTranslatef(3.0f,0.0f,0.0f);					// Move Right 3 Units

    glBegin(GL_QUADS);						// Draw A Quad
      glVertex3f(-1.0f, 1.0f, 0.0f);				// Top Left
      glVertex3f( 1.0f, 1.0f, 0.0f);				// Top Right
      glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
      glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
    glEnd();*/

  XTransform t = XTransformUtilities::lookAt(XVector3D(10,5,10), XVector3D::Zero(), XVector3D(0,1,0));

  glLoadIdentity();						// Reset The View
  r->pushTransform(t);

  glBegin(GL_TRIANGLES);						// Drawing Using Triangles
    glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
    glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
    glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
  glEnd();							// Finished Drawing The Triangle

  glTranslatef(3.0f,0.0f,0.0f);					// Move Right 3 Units

  glBegin(GL_QUADS);						// Draw A Quad
    glVertex3f(-1.0f, 1.0f, 0.0f);				// Top Left
    glVertex3f( 1.0f, 1.0f, 0.0f);				// Top Right
    glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
    glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
  glEnd();

  XGeometry x;
  XModeller m(&x, 128);

  m.drawCube();

  XShader s;

  r->setShader(&s);
  r->drawGeometry(x);
  r->setShader(0);

  r->popTransform();
  }
