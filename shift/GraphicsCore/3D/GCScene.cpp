#include "GCScene.h"
#include "XModeller.h"
#include "XShader.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCScene)

SPropertyInformation *GCScene::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCScene>("GCScene");

  info->add(&GCScene::camera, "camera");

  return info;
  }

GCScene::GCScene()
  {
  }

void GCScene::render(XRenderer *r) const
  {
  if(camera())
    {
    camera()->begin(r);

    XGeometry x;
    XModeller m(&x, 128);

    m.drawCube();

    XShader s;

    r->setShader(&s);
    r->drawGeometry(x);
    r->setShader(0);

    camera()->end(r);
    }
  }
