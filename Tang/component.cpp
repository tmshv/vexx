#include "component.h"
#include "spropertyinformationhelpers.h"
#include "componentdocument.h"
#include "tangmanager.h"

S_IMPLEMENT_PROPERTY(Component, Tang);

void Component::createTypeInformation(SPropertyInformationTyped<Component> *info,
                                      const SPropertyInformationCreateData &data)
  {
  if(data.registerInterfaces)
    {
    info->addStaticInterface(new ComponentEditorInterface);
    }
  }

Component::Component()
  {
  }

class SelectorInternal : public GCRenderable::Selector
  {
public:
  SelectorInternal(GCRenderable::Selector *parent, Component *c) : _parent(parent), _component(c)
    {
    }

  void onHit(const XVector3D &point, const XVector3D &normal, GCRenderable *)
    {
    _parent->onHit(point, normal, _component);
    }

private:
  GCRenderable::Selector *_parent;
  Component *_component;
  };

void Component::intersect(const XLine &line, Selector *s)
  {
  SelectorInternal sel(s, this);
  GCRenderArray::intersect(line, &sel);
  }

void Component::intersect(const XFrustum &frus, Selector *s)
  {
  SelectorInternal sel(s, this);
  GCRenderArray::intersect(frus, &sel);
  }
