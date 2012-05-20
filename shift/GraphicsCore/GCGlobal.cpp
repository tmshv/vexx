#include "GCGlobal.h"
#include "styperegistry.h"

#include "sbaseproperties.h"
#include "3D/GCTexture.h"
#include "3D/GCShaderDataBindings.h"
#include "spropertyinformationhelpers.h"

namespace GraphicsCore
{
void initiate()
  {
  STypeRegistry::addPropertyGroup(propertyGroup());

  SPropertyInformation *colInfo = const_cast<SPropertyInformation *>(ColourProperty::staticTypeInformation());
  colInfo->addStaticInterface(new GCShaderDataBindings::Vector4);

  SPropertyInformation *texInfo = const_cast<SPropertyInformation *>(GCTexturePointer::staticTypeInformation());
  texInfo->addStaticInterface(new GCShaderDataBindings::TextureRef);
  }

SPropertyGroup &propertyGroup()
  {
  static SPropertyGroup grp;
  return grp;
  }
}
