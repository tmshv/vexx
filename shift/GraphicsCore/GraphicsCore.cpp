#include "GraphicsCore.h"
#include "GCImage.h"
#include "GCQImage.h"
#include "3D/GCGeometry.h"
#include "3D/GCRenderToScreen.h"
#include "3D/GCRenderToTexture.h"
#include "3D/GCRenderTarget.h"
#include "3D/GCCamera.h"
#include "3D/GCTransform.h"
#include "styperegistry.h"

void initiateGraphicsCore(SDatabase *db)
  {
  STypeRegistry::addType(GCImage::staticTypeInformation());
  STypeRegistry::addType(GCQImage::staticTypeInformation());

  STypeRegistry::addType(GCPolygonArray::staticTypeInformation());
  STypeRegistry::addType(GCGeometry::staticTypeInformation());

  STypeRegistry::addType(GCTransform::staticTypeInformation());
  STypeRegistry::addType(GCCamera::staticTypeInformation());

  STypeRegistry::addType(GCRenderTarget::staticTypeInformation());
  STypeRegistry::addType(GCRenderToScreen::staticTypeInformation());
  STypeRegistry::addType(GCRenderToTexture::staticTypeInformation());
  }
