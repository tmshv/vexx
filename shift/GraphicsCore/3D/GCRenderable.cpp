#include "GCRenderable.h"
#include "spropertyinformationhelpers.h"

S_IMPLEMENT_TYPED_POINTER_TYPE(GCRenderablePointer, GraphicsCore)
S_IMPLEMENT_TYPED_POINTER_ARRAY_TYPE(GCRenderablePointerArray, GraphicsCore)

S_IMPLEMENT_PROPERTY(GCRenderable, GraphicsCore)

void GCRenderable::createTypeInformation(SPropertyInformationTyped<GCRenderable> *info,
                                         const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCRenderable::bounds, "bounds");
    }
  }

GCRenderable::GCRenderable()
  {
  }

void GCRenderable::intersect(const XLine &, Selector *)
  {
  }

void GCRenderable::intersect(const XFrustum &, Selector *)
  {
  }


S_IMPLEMENT_PROPERTY(GCRenderArray, GraphicsCore)

void unionBounds(const SPropertyInstanceInformation*, GCRenderArray* array)
  {
  GCBoundingBox::ComputeLock l(&array->bounds);
  XCuboid *data = l.data();
  *data = XCuboid();

  for(GCRenderablePointer* r = array->renderGroup.firstChild<GCRenderablePointer>(); r; r = r->nextSibling<GCRenderablePointer>())
    {
    const GCRenderable* ptd = r->pointed();

    data->unite(ptd->bounds());
    }
  }

void GCRenderArray::createTypeInformation(SPropertyInformationTyped<GCRenderArray> *info,
                                          const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    auto rGInst = info->add(&GCRenderArray::renderGroup, "renderGroup");

    auto bInst = info->child(&GCRenderArray::bounds);
    bInst->setCompute<unionBounds>();

    rGInst->setAffects(bInst);
    }
  }

GCRenderArray::GCRenderArray()
  {
  }

void GCRenderArray::render(XRenderer *renderer) const
  {
  for(GCRenderablePointer* r = renderGroup.firstChild<GCRenderablePointer>(); r; r = r->nextSibling<GCRenderablePointer>())
    {
    GCRenderable* ptd = r->pointed();
    if(ptd)
      {
      ptd->render(renderer);
      }
    }
  }

void GCRenderArray::intersect(const XLine &line, Selector *s)
  {
  for(GCRenderablePointer* r = renderGroup.firstChild<GCRenderablePointer>(); r; r = r->nextSibling<GCRenderablePointer>())
    {
    GCRenderable* ptd = r->pointed();
    if(ptd)
      {
      ptd->intersect(line, s);
      }
    }
  }

void GCRenderArray::intersect(const XFrustum &frus, Selector *s)
  {
  for(GCRenderablePointer* r = renderGroup.firstChild<GCRenderablePointer>(); r; r = r->nextSibling<GCRenderablePointer>())
    {
    GCRenderable* ptd = r->pointed();
    if(ptd)
      {
      ptd->intersect(frus, s);
      }
    }
  }
