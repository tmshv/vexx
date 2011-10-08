#include "sloader.h"
#include "spropertycontainer.h"
#include "spropertyinformation.h"

void SLoader::loadChildren(SPropertyContainer *parent)
  {
  beginChildren();
  while(hasNextChild())
    {
    beginNextChild();
    read(parent);
    endNextChild();
    }
  endChildren();
  }

void SSaver::saveChildren(const SPropertyContainer *c)
  {
  beginChildren();
  for(const SProperty *child=c->firstChild(); child; child=child->nextSibling())
    {
    const SPropertyInformation *info = child->typeInformation();

    if(info->shouldSave()(child))
      {
      beginNextChild();
      write(child);
      endNextChild();
      }
    }
  endChildren();
  }
