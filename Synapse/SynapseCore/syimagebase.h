#ifndef SYIMAGEBASE_H
#define SYIMAGEBASE_H

#include "syglobal.h"
#include "sbaseproperties.h"
#include "sarrayproperty.h"

class QImage;

class SyImageBase : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(SyImageBase, SPropertyContainer, 0);
public:
  SyImageBase();

  SFloatArrayProperty image;
  void loadImage(const QImage& );
  };

#endif // SYIMAGEBASE_H
