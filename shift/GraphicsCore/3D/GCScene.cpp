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

  r->drawGeometry(x);
  r->setShader(0);

  r->popTransform();
  }
