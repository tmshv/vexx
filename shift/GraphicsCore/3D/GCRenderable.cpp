#include "GCRenderable.h"

S_IMPLEMENT_PROPERTY(GCRenderable)

void GCRenderable::createTypeInformation(SPropertyInformation *, const SPropertyInformationCreateData &)
  {
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


S_IMPLEMENT_PROPERTY(GCRenderArray)

void GCRenderArray::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCRenderArray::renderGroup, "renderGroup");
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
    ptd->render(renderer);
    }
  }

void GCRenderArray::intersect(const XLine &line, Selector *s)
  {
  for(GCRenderablePointer* r = renderGroup.firstChild<GCRenderablePointer>(); r; r = r->nextSibling<GCRenderablePointer>())
    {
    GCRenderable* ptd = r->pointed();
    ptd->intersect(line, s);
    }
  }

void GCRenderArray::intersect(const XFrustum &frus, Selector *s)
  {
  for(GCRenderablePointer* r = renderGroup.firstChild<GCRenderablePointer>(); r; r = r->nextSibling<GCRenderablePointer>())
    {
    GCRenderable* ptd = r->pointed();
    ptd->intersect(frus, s);
    }
  }
