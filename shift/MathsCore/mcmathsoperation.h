#ifndef MCMATHSOPERATION_H
#define MCMATHSOPERATION_H

#include "mcglobal.h"
#include "sbaseproperties.h"
#include "XArrayMath"

class MATHSCORE_EXPORT MCMathsOperation : public SPODPropertyBase<XMathsOperation, MCMathsOperation>
  {
  S_PROPERTY(MCMathsOperation, SProperty, 0)

public:
  static void assignProperty(const SProperty *, SProperty *);

  bool saveResultToFile(QString filename);
  QImage asQImage() const;
  };

#endif // MCMATHSOPERATION_H
