#ifndef SINGLELEVELRENDERMODEL_H
#define SINGLELEVELRENDERMODEL_H

#include "XAbstractRenderModel.h"
#include "GCAbstractNodeIterator.h"
#include "GCGlobal.h"
#include "sobserver.h"
#include "sentityweakpointer.h"

class SEntity;

class GRAPHICSCORE_EXPORT GCSingleLevelRenderModel : public XAbstractRenderModel, SConnectionObserver, STreeObserver
  {
XProperties:
  XROProperty(SEntityWeakPointer, entity);

public:
  class GRAPHICSCORE_EXPORT Iterator : public GCAbstractNodeIterator
    {
  XProperties:
    XROProperty(const GCSingleLevelRenderModel *, model);

  public:
    Iterator(const GCSingleLevelRenderModel *model);
    virtual bool next();

    friend class GCSingleLevelRenderModel;
    };

  GCSingleLevelRenderModel(SEntity *ent=0);
  ~GCSingleLevelRenderModel();

  void setEntity(SEntity *entity);

  virtual XAbstractRenderModel::Iterator *createIterator() const;
  virtual void destroyIterator(XAbstractRenderModel::Iterator *) const;

  virtual void resetIterator(XAbstractRenderModel::Iterator *) const;

  void onConnectionChange(int m, const SChange *);
  void onTreeChange(int m, const SChange *);
  };

#endif // SINGLELEVELRENDERMODEL_H
