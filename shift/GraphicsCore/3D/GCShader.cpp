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

void GCShader::computeShaderRuntime(const SPropertyInstanceInformation *info, SPropertyContainer *cont)
  {
  GCShader* shader = cont->uncheckedCastTo<GCShader>();

  GCRuntimeShader::ComputeLock lock(&shader->runtimeShader);

  if(shader->_rebuildShader)
    {
    lock.data()->clear();
    for(const GCShaderComponentPointer* cmpPtr = shader->components.firstChild<GCShaderComponentPointer>(); cmpPtr; cmpPtr = cmpPtr->nextSibling<GCShaderComponentPointer>())
      {
      const GCShaderComponent* cmp = cmpPtr->pointed();
      if(cmp)
        {
        XAbstractShader::ComponentType t;
        if(cmp->castTo<GCFragmentShaderComponent>())
          {
          t = XAbstractShader::Fragment;
          }
        else if(cmp->castTo<GCVertexShaderComponent>())
          {
          t = XAbstractShader::Vertex;
          }
        else
          {
          xAssertFail();
          }
        lock.data()->addComponent(t, cmp->source());
        }
      }
    }
  else
    {
    xAssert(shader->_setVariables);
    for(const SProperty* p = shader->runtimeShader.nextSibling(); p; p = p->nextSibling())
      {
      const GCShaderBindableData *binder = p->interface<GCShaderBindableData>();
      if(binder)
        {
        binder->bindData(lock.data(), p);
        }
      }
    }
  }

SPropertyInformation *GCShader::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCShader>("GCShader");

  GCRuntimeShader::InstanceInformation *rtInfo = info->add(&GCShader::runtimeShader, "runtimeShader");
  rtInfo->setCompute(computeShaderRuntime);

  GCShaderComponentPointerArray::InstanceInformation *comInfo = info->add(&GCShader::components, "components");
  comInfo->setAffects(rtInfo);

  return info;
  }

GCShader::GCShader()
  {
  _setVariables = true;
  _rebuildShader = true;
  }

void GCShader::bind(XRenderer *r) const
  {
  r->setShader(&runtimeShader());
  }

void GCShader::postChildSet(SPropertyContainer *c, SProperty *p)
  {
  ParentType::postChildSet(c, p);

  GCShader *shader = c->uncheckedCastTo<GCShader>();

  if(p == &shader->components)
    {
    shader->_rebuildShader = true;
    }
  else
    {
    shader->_setVariables = true;
    }
  }
