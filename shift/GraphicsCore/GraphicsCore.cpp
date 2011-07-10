#include "GraphicsCore.h"
#include "styperegistry.h"

#include "GCImage.h"
#include "GCQImage.h"

#include "3D/GCGeometry.h"
#include "3D/GCRenderToScreen.h"
#include "3D/GCRenderToTexture.h"
#include "3D/GCRenderTarget.h"

#include "3D/GCCamera.h"
#include "3D/GCTransform.h"
#include "3D/GCRenderable.h"
#include "3D/GCScene.h"

#include "3D/Renderable/GCGrid.h"

#include "3D/GCViewport.h"


void initiateGraphicsCore(SDatabase *db)
  {
  STypeRegistry::addType(GCImage::staticTypeInformation());
  STypeRegistry::addType(GCQImage::staticTypeInformation());

  STypeRegistry::addType(GCPolygonArray::staticTypeInformation());
  STypeRegistry::addType(GCGeometry::staticTypeInformation());

  STypeRegistry::addType(GCTransform::staticTypeInformation());

  STypeRegistry::addType(GCViewport::staticTypeInformation());

  STypeRegistry::addType(GCCamera::staticTypeInformation());
  STypeRegistry::addType(GCPerspectiveCamera::staticTypeInformation());

  STypeRegistry::addType(GCRenderTarget::staticTypeInformation());
  STypeRegistry::addType(GCRenderToScreen::staticTypeInformation());
  STypeRegistry::addType(GCRenderToTexture::staticTypeInformation());

  STypeRegistry::addType(GCRenderable::staticTypeInformation());

  STypeRegistry::addType(GCGrid::staticTypeInformation());
  STypeRegistry::addType(GCScene::staticTypeInformation());
  }
