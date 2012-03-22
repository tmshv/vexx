#include "GCShader.h"
#include "XRenderer.h"
#include "sprocessmanager.h"

S_IMPLEMENT_PROPERTY(GCShaderComponent)

void GCShaderComponent::createTypeInformation(SPropertyInformation *, const SPropertyInformationCreateData &)
  {
  }

GCShaderComponent::GCShaderComponent()
  {
  }

S_IMPLEMENT_PROPERTY(GCFragmentShaderComponent)

void GCFragmentShaderComponent::createTypeInformation(SPropertyInformation *, const SPropertyInformationCreateData &)
  {
  }

S_IMPLEMENT_PROPERTY(GCVertexShaderComponent)

void GCVertexShaderComponent::createTypeInformation(SPropertyInformation *, const SPropertyInformationCreateData &)
  {
  }

S_IMPLEMENT_PROPERTY(GCShader)

void GCShader::computeShaderRuntime(const SPropertyInstanceInformation *, GCShader *shader)
  {
  xAssert(SProcessManager::isMainThread());

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
        lock.data()->addComponent(t, cmp->value());
        }
      }

    shader->_setVariables = true;
    }

  if(shader->_setVariables)
    {
    for(const SProperty* p = shader->components.nextSibling(); p; p = p->nextSibling())
      {
      const GCShaderBindableData *binder = p->interface<GCShaderBindableData>();
      if(binder)
        {
        binder->bindData(lock.data(), p);
        }
      }
    }

  shader->_rebuildShader = false;
  shader->_setVariables = false;
  }

void GCShader::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    GCRuntimeShader::InstanceInformation *rtInfo = info->add(&GCShader::runtimeShader, "runtimeShader");
    rtInfo->setCompute(computeShaderRuntime);
    rtInfo->setComputeLockedToMainThread(true);

    GCShaderComponentPointerArray::InstanceInformation *comInfo = info->add(&GCShader::components, "components");
    comInfo->setAffects(rtInfo);
    }
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

  if(p == &shader->runtimeShader)
    {
    return;
    }

  if(shader->components.isDirty())
    {
    shader->_rebuildShader = true;
    }
  else
    {
    shader->_setVariables = true;
    }
  }
