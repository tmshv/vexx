#include "GCSingleLevelRenderModel.h"
#include "sentity.h"

GCSingleLevelRenderModel::Iterator::Iterator(const GCSingleLevelRenderModel *m) : _model(m)
  {
  }

bool GCSingleLevelRenderModel::Iterator::next()
  {
  xAssert(_model);
  if(entity())
    {
    setEntity(entity()->nextSibling<SEntity>());
    }
  else
    {
    setEntity(_model->entity()->children.firstChild<SEntity>());
    }

  return entity() != 0;
  }

GCSingleLevelRenderModel::GCSingleLevelRenderModel(SEntity *ent) : _entity(0)
  {
  setEntity(ent);
  }

GCSingleLevelRenderModel::~GCSingleLevelRenderModel()
  {
  setEntity(0);
  }

void GCSingleLevelRenderModel::setEntity(SEntity *entity)
  {
  if(_entity)
    {
    _entity->removeTreeObserver(this);

    for(SEntity *child=_entity->children.firstChild<SEntity>(); child; child=child->nextSibling<SEntity>())
      {
      child->removeConnectionObserver(this);
      }
    }

  _entity = entity;

  if(_entity)
    {
    _entity->addTreeObserver(this);

    for(SEntity *child=_entity->children.firstChild<SEntity>(); child; child=child->nextSibling<SEntity>())
      {
      child->addConnectionObserver(this);
      }
    }
  }

XAbstractRenderModel::Iterator *GCSingleLevelRenderModel::createIterator() const
  {
  return new GCSingleLevelRenderModel::Iterator(this);
  }

void GCSingleLevelRenderModel::destroyIterator(XAbstractRenderModel::Iterator *it) const
  {
  delete it;
  }

void GCSingleLevelRenderModel::resetIterator(XAbstractRenderModel::Iterator *it) const
  {
  GCSingleLevelRenderModel::Iterator *slIt = static_cast<GCSingleLevelRenderModel::Iterator*>(it);
  slIt->setEntity(0);
  }

void GCSingleLevelRenderModel::onConnectionChange(int m, const SChange *)
  {
  update(TreeChange);
  }

void GCSingleLevelRenderModel::onTreeChange(int m, const SChange *c)
  {
  update(TreeChange);

  const SPropertyContainer::TreeChange *t = c->castTo<SPropertyContainer::TreeChange>();
  if(t)
    {
    if(t->before() && t->before()->isDescendedFrom(_entity))
      {
      t->property()->entity()->removeConnectionObserver(this);
      }
    if(t->after() && t->after()->isDescendedFrom(_entity))
      {
      t->property()->entity()->addConnectionObserver(this);
      }
    }
  }
