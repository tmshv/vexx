#ifndef SINGLELEVELRENDERMODEL_H
#define SINGLELEVELRENDERMODEL_H

#include "XAbstractRenderModel.h"
#include "GCGlobal.h"

class SEntity;

class GRAPHICSCORE_EXPORT GCSingleLevelRenderModel : public XAbstractRenderModel
  {
properties:
  XROProperty(SEntity *, entity);

public:
  class GRAPHICSCORE_EXPORT Iterator : public XAbstractRenderModel::Iterator
    {
  properties:
    XProperty(SEntity *, entity, setEntity);

    virtual bool next();
    };

  GCSingleLevelRenderModel(SEntity *ent);

  void setEntity(SEntity *entity);

  virtual Iterator *createIterator() const;
  virtual void destroyIterator(Iterator *) const;

  virtual void resetIterator(Iterator *) const;
  };

#endif // SINGLELEVELRENDERMODEL_H
