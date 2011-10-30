#ifndef SAPARTEDITOR_H
#define SAPARTEDITOR_H

#include "saglobal.h"
#include "QWidget"

class SProperty;
class SPropertyContainer;

class SHIFTALTER_EXPORT SPartEditor : public QWidget
  {
public:
  SPartEditor(const QString &type, SProperty *prop);

  static SPartEditor *editNewPart(const QString &type, const QString &name, SEntity *parent);
  };

#endif // SAPARTEDITOR_H
