#include "GCShader.h"
#include "XRenderer.h"

S_IMPLEMENT_PROPERTY(GCShaderComponent)

SPropertyInformation *GCShaderComponent::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShaderComponent>("GCShaderComponent");

  info->add(&GCShaderComponent::source, "source");

  return info;
  }

GCShaderComponent::GCShaderComponent()
  {
  }

S_IMPLEMENT_PROPERTY(GCFragmentShaderComponent)

SPropertyInformation *GCFragmentShaderComponent::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCFragmentShaderComponent>("GCFragmentShaderComponent");
  return info;
  }

S_IMPLEMENT_PROPERTY(GCVertexShaderComponent)

SPropertyInformation *GCVertexShaderComponent::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCVertexShaderComponent>("GCVertexShaderComponent");
  return info;
  }

S_IMPLEMENT_PROPERTY(GCShader)

SPropertyInformation *GCShader::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShader>("GCShader");

  info->add(&GCShader::components, "components");

  return info;
  }

GCShader::GCShader()
  {
  }

void GCShader::bind(XRenderer *r) const
  {
  r->setShader(&shader);
  }
