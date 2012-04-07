#include "GCRenderable.h"

S_IMPLEMENT_PROPERTY(GCRenderable)

void GCRenderable::createTypeInformation(SPropertyInformation *, const SPropertyInformationCreateData &)
  {
  }

GCRenderable::GCRenderable()
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
    if(ptd)
      {
      ptd->render(renderer);
      }
    }
  }
