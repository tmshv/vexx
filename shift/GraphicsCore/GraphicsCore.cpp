#include "GraphicsCore.h"
#include "styperegistry.h"

#include "GCImage.h"

#include "3D/GCGeometry.h"
#include "3D/GCScreenRenderTarget.h"
#include "3D/GCTextureRenderTarget.h"
#include "3D/GCTextureRenderTarget.h"

#include "3D/GCCamera.h"
#include "3D/GCTransform.h"
#include "3D/GCRenderable.h"
#include "3D/GCScene.h"

#include "3D/Renderable/GCGrid.h"

#include "3D/GCViewport.h"

#include "3D/GCShader.h"
#include "3D/GCShadingGroup.h"

#include "3D/Shaders/GCStandardSurface.h"


void initiateGraphicsCore(SDatabase *)
  {

  STypeRegistry::addType(GCImage::staticTypeInformation());
  STypeRegistry::addType(GCQImage::staticTypeInformation());

  STypeRegistry::addType(GCGeometryAttribute::staticTypeInformation());
  STypeRegistry::addType(GCGeometry::staticTypeInformation());

  STypeRegistry::addType(GCTransform::staticTypeInformation());

  STypeRegistry::addType(GCViewport::staticTypeInformation());

  STypeRegistry::addType(GCViewableTransform::staticTypeInformation());
  STypeRegistry::addType(GCCamera::staticTypeInformation());
  STypeRegistry::addType(GCPerspectiveCamera::staticTypeInformation());


  STypeRegistry::addType(GCRenderTarget::staticTypeInformation());
  STypeRegistry::addType(GCScreenRenderTarget::staticTypeInformation());
  STypeRegistry::addType(GCTextureRenderTarget::staticTypeInformation());

  STypeRegistry::addType(GCRenderable::staticTypeInformation());

  STypeRegistry::addType(GCGrid::staticTypeInformation());
  STypeRegistry::addType(GCScene::staticTypeInformation());


  STypeRegistry::addType(GCShadingGroup::staticTypeInformation());
  STypeRegistry::addType(GCShader::staticTypeInformation());

  STypeRegistry::addType(GCStandardSurface::staticTypeInformation());
  }
