#ifndef SYIMAGEBASE_H
#define SYIMAGEBASE_H

#include "syglobal.h"
#include "sbaseproperties.h"
#include "sarrayproperty.h"

class QImage;

class SYNAPSECORE_EXPORT SyImageBase : public SPropertyContainer
  {
  S_PROPERTY_CONTAINER(SyImageBase, SPropertyContainer, 0);
public:
  SyImageBase();

  SFloatArrayProperty image;

  void loadQImage(const QImage &);
  QImage asQImage() const;
  };

#endif // SYIMAGEBASE_H
