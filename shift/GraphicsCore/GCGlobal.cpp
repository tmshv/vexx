#include "GCGlobal.h"
#include "styperegistry.h"

#include "sbaseproperties.h"
#include "3D/GCTexture.h"
#include "3D/GCShaderDataBindings.h"

namespace GraphicsCore
{
void initiate()
  {
  STypeRegistry::addPropertyGroup(propertyGroup());

  ColourProperty::staticTypeInformation()->addStaticInterface(new GCShaderDataBindings::Vector4);
  GCTexturePointer::staticTypeInformation()->addStaticInterface(new GCShaderDataBindings::TextureRef);
  }

SPropertyGroup &propertyGroup()
  {
  static SPropertyGroup grp;
  return grp;
  }
}
