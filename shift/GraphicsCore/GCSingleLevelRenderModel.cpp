#include "GCSingleLevelRenderModel.h"
#include "sentity.h"

bool GCSingleLevelRenderModel::Iterator::next()
  {
  if(_entity)
    {
    _entity = _entity->nextSibling<SEntity>();
    }

  return _entity != 0;
  }

GCSingleLevelRenderModel::GCSingleLevelRenderModel(SEntity *ent) : _entity(0)
  {
  setEntity(ent);
  }

void GCSingleLevelRenderModel::setEntity(SEntity *entity)
  {
  _entity = entity;
  }

GCSingleLevelRenderModel::Iterator *GCSingleLevelRenderModel::createIterator() const
  {
  return new GCSingleLevelRenderModel::Iterator();
  }

void GCSingleLevelRenderModel::destroyIterator(Iterator *it) const
  {
  delete it;
  }

void GCSingleLevelRenderModel::resetIterator(Iterator *it) const
  {
  GCSingleLevelRenderModel::Iterator *slIt = static_cast<GCSingleLevelRenderModel::Iterator*>(it);
  if(_entity)
    {
    slIt->setEntity(_entity->firstChild<SEntity>());
    }
  else
    {
    slIt->setEntity(0);
    }
  }
