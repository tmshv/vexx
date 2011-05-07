#ifndef GCSHIFTRENDERMODEL_H
#define GCSHIFTRENDERMODEL_H

#include "XAbstractRenderModel.h"
#include "GCGlobal.h"
#include "sobserver.h"
#include "sentityweakpointer.h"
#include "siterator.h"

class SEntity;

class GRAPHICSCORE_EXPORT GCShiftRenderModel : public XAbstractRenderModel, SConnectionObserver, STreeObserver
  {
XProperties:
  XROProperty(SEntityWeakPointer, entity);
  XROProperty(SIterator::FilterFunction, filter);

public:
  class GRAPHICSCORE_EXPORT Iterator : public XAbstractRenderModel::Iterator
    {
  XProperties:
    XROProperty(const GCShiftRenderModel *, model);
    XROProperty(SIterator::DataCache, cache);
    XROProperty(SProperty *, property);

  public:
    Iterator(const GCShiftRenderModel *model);
    virtual bool next();

    SEntity *entity() const
      {
      if(property())
        {
        SEntity *ent = property()->castTo<SEntity>();
        xAssert(ent);
        return ent;
        }
      return 0;
      }

  protected:
    void reset();
    void setProperty(SProperty *p) { _property = p; }

    friend class GCShiftRenderModel;
    };

  GCShiftRenderModel(SEntity *ent=0, SIterator::FilterFunction=0);
  ~GCShiftRenderModel();

  void setEntity(SEntity *entity);

  virtual XAbstractRenderModel::Iterator *createIterator() const;
  virtual void destroyIterator(XAbstractRenderModel::Iterator *) const;

  virtual void resetIterator(XAbstractRenderModel::Iterator *) const;

  void onConnectionChange(int m, const SChange *);
  void onTreeChange(int m, const SChange *);
  };

#endif // GCSHIFTRENDERMODEL_H
