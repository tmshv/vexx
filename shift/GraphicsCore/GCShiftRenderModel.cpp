#include "GCShiftRenderModel.h"
#include "sentity.h"

GCShiftRenderModel::Iterator::Iterator(const GCShiftRenderModel *m) : _model(m), _cache(0)
  {
  }

bool GCShiftRenderModel::Iterator::next()
  {
  xAssert(_model);

  SIterator::FilterFunction filter(_model->filter());
  xAssert(filter);

  setProperty(filter(_cache));
  return property() != 0;
  }

void GCShiftRenderModel::Iterator::reset()
  {
  _property = 0;
  _cache.reset(_model->entity());
  }

GCShiftRenderModel::GCShiftRenderModel(SEntity *ent, SIterator::FilterFunction func) : _entity(0), _filter(func)
  {
  xAssert(func);
  setEntity(ent);
  }

GCShiftRenderModel::~GCShiftRenderModel()
  {
  setEntity(0);
  }

void GCShiftRenderModel::setEntity(SEntity *entity)
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

XAbstractRenderModel::Iterator *GCShiftRenderModel::createIterator() const
  {
  return new GCShiftRenderModel::Iterator(this);
  }

void GCShiftRenderModel::destroyIterator(XAbstractRenderModel::Iterator *it) const
  {
  delete it;
  }

void GCShiftRenderModel::resetIterator(XAbstractRenderModel::Iterator *it) const
  {
  GCShiftRenderModel::Iterator *slIt = static_cast<GCShiftRenderModel::Iterator*>(it);
  slIt->reset();
  }

void GCShiftRenderModel::onConnectionChange(int m, const SChange *)
  {
  update(TreeChange);
  }

void GCShiftRenderModel::onTreeChange(int m, const SChange *c)
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
