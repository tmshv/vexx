#include "GCShader.h"
#include "XRenderer.h"

void writeValue(SSaver &s, const XShader &t)
  {
  xAssertFail();
  }

void readValue(SLoader &l, XShader &t)
  {
  xAssertFail();
  }

IMPLEMENT_POD_PROPERTY(GCRuntimeShader, XShader)

void GCRuntimeShader::assignProperty(const SProperty *f, SProperty *t)
  {
  GCRuntimeShader *to = t->uncheckedCastTo<GCRuntimeShader>();

  const GCRuntimeShader *sProp = f->castTo<GCRuntimeShader>();
  if(sProp)
    {
    to->assign(sProp->value());
    return;
    }
  }

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

  if(shader->_rebuildShader)
    {
    XShader rtShader;
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
        rtShader.addComponent(t, cmp->source());
        }
      }
    shader->runtimeShader = rtShader;
    }
  else
    {
    GCRuntimeShader::Lock lock(&shader->runtimeShader);
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
