#ifndef SYIMAGEBASE_H
#define SYIMAGEBASE_H

#include "syglobal.h"
#include "sbaseproperties.h"
#include "sarrayproperty.h"
#include "mcmathsoperation.h"

class QImage;

class SYNAPSECORE_EXPORT SyImageBase : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(SyImageBase, SPropertyContainer, 0);

public:
  SyImageBase();

  FloatProperty time;
  MCMathsOperation operation;

  void loadQImage(const QImage &, bool premult);
  QImage asQImage() const;

private:
  XMathsOperation _preOperation;
  };

#endif // SYIMAGEBASE_H
