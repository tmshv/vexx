#ifndef MCIMAGE_H
#define MCIMAGE_H

#include "mcsource.h"
#include "sbaseproperties.h"

class MATHSCORE_EXPORT MCImage : public MCSource
  {
  S_ENTITY(MCImage, MCSource, 0);

public:
  MCImage();

  BoolProperty premultiply;
  StringProperty filename;

private:
  static void computeImageOutput(const SPropertyInstanceInformation* inst, MCImage *image);
  XMathsOperation _preOperation;
  };

#endif // MCIMAGE_H
