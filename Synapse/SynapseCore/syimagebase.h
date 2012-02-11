#ifndef SYIMAGEBASE_H
#define SYIMAGEBASE_H

#include "syglobal.h"
#include "sbaseproperties.h"
#include "sarrayproperty.h"
#include "mcmathsoperation.h"

class QImage;

class SYNAPSECORE_EXPORT SyImageBase : public MCMathsOperation
  {
  S_PROPERTY(SyImageBase, MCMathsOperation, 0);

public:
  SyImageBase();

  void loadQImage(const QImage &, bool premult);
  QImage asQImage() const;

private:
  XMathsOperation _preOperation;
  };

#endif // SYIMAGEBASE_H
