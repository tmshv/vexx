#ifndef STYPEREGISTRY_H
#define STYPEREGISTRY_H

#include "sglobal.h"
#include "XSet"
#include "spropertyinformation.h"

class SHIFT_EXPORT STypeRegistry
  {
public:
  static void initiate();

  static const XSet <const SPropertyInformation *> &types();
  static void addType(const SPropertyInformation *);

  static const SPropertyInformation *findType(const QString &);

private:
  STypeRegistry();
  X_DISABLE_COPY(STypeRegistry)
  };

#endif // STYPEREGISTRY_H
