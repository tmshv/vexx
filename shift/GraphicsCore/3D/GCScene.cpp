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
    {
    XModeller m(&x, 128);

    m.begin(XModeller::Triangles);
      m.vertex( 0.0f, 1.0f, 0.0f);
      m.vertex(-1.0f,-1.0f, 0.0f);
      m.vertex( 1.0f,-1.0f, 0.0f);
    m.end();
    }
  }

void GCScene::render(XRenderer *r) const
  {
  r->setProjectionTransform(cameraProjection());

  XTransform t = XTransformUtilities::lookAt(XVector3D(10,5,10), XVector3D::Zero(), XVector3D(0,1,0));

  glLoadIdentity();						// Reset The View
  r->pushTransform(t);

  r->setShader(&s);


  glBegin(GL_LINES);
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,-1.0f, 0.0f);
    glVertex3f( 1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f( 0.0f, 0.0f, 1.0f);
    glVertex3f( 0.0f, 0.0f,-1.0f);
  glEnd();

  glBegin(GL_TRIANGLES);						// Drawing Using Triangles
    glVertex3f( 0.0f,-2.0f, 0.0f);				// Top
    glVertex3f(-1.0f,-4.0f, 0.0f);				// Bottom Left
    glVertex3f( 1.0f,-4.0f, 0.0f);				// Bottom Right
  glEnd();							// Finished Drawing The Triangle

  glTranslatef(3.0f,0.0f,0.0f);					// Move Right 3 Units

  glBegin(GL_QUADS);						// Draw A Quad
    glVertex3f(-1.0f, 1.0f, 0.0f);				// Top Left
    glVertex3f( 1.0f, 1.0f, 0.0f);				// Top Right
    glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
    glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
  glEnd();



  r->drawGeometry(x);
  r->setShader(0);

  r->popTransform();
  }
