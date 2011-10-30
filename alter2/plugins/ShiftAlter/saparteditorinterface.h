#ifndef SAPARTEDITORINTERFACE_H
#define SAPARTEDITORINTERFACE_H

#include "saglobal.h"
#include "sinterface.h"

class SPartEditorInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(SPartEditorInterface, SPartEditorInterface)

public:
  SPartEditorInterface(bool deleteOnNoReferences) : SStaticInterfaceBase(deleteOnNoReferences) { }
  virtual ~SPartEditorInterface() { }

  virtual xsize numberOfTypeParameters(SProperty *prop) const = 0;
  virtual void typeParameter(SProperty *prop, xsize index, QString& name, QWidget *&widget) const = 0;

  virtual bool hasPropertiesSection() const = 0;
  virtual void propertiesSectionTitle(QString &) const = 0;

  virtual void properties(SPropertyContainer *, QStringList &) const = 0;
  };

class SDefaultPartEditorInterface : public SPartEditorInterface
  {
public:
  SDefaultPartEditorInterface();

  enum
    {
    Name,
    Type,
    NumberOfTypeParameters
    };

  virtual xsize numberOfTypeParameters(SProperty *prop) const;
  virtual void typeParameter(SProperty *prop, xsize index, QString& name, QWidget *&widget) const;

  virtual bool hasPropertiesSection() const;
  virtual void propertiesSectionTitle(QString &n) const;

  virtual void properties(SPropertyContainer *, QStringList &) const;
  };

#endif // SAPARTEDITORINTERFACE_H
