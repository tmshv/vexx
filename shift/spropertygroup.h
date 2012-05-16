#ifndef SPROPERTYGROUP_H
#define SPROPERTYGROUP_H

#include "sglobal.h"
#include "XProperty"
#include "QVector"

class SPropertyInformation;

class SHIFT_EXPORT SPropertyGroup
  {
XProperties:
  XRORefProperty(QVector<const SPropertyInformation *>, containedTypes);

public:
  SPropertyGroup();

  void registerPropertyInformation(const SPropertyInformation *);

private:
  bool _added;
  friend class STypeRegistry;
  };

#endif // SPROPERTYGROUP_H
