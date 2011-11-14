#ifndef SASHADERPARTEDITORINTERFACE_H
#define SASHADERPARTEDITORINTERFACE_H

#include "saparteditorinterface.h"

class SShaderPartEditorInterface : public SDefaultPartEditorInterface
  {
public:
  enum
    {
    SubValue = SDefaultPartEditorInterface::NumberOfSubTypeParameters,
    NumberOfSubTypeParameters
    };

  virtual xsize numberOfTypeParameters(SEntity *prop) const;

  virtual void addProperty(SEntity *) const;
  virtual QStringList possiblePropertyTypes() const;

  virtual xsize numberOfTypeSubParameters(SEntity *, void *property) const;
  virtual void typeSubParameter(SPartEditorInterfaceFeedbacker *, SEntity *, void *prop, xsize i, QString &name, QWidget *&widget) const;

  virtual QWidget *buildCustomEditor(SEntity *) const;
  virtual QWidget *buildCustomPreview(const SEntity *, SEntity *) const;
  };

#endif // SASHADERPARTEDITORINTERFACE_H
