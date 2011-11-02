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

  virtual xsize numberOfTypeParameters(SEntity *prop) const = 0;
  virtual void typeParameter(SEntity *prop, xsize index, QString& name, QWidget *&widget) const = 0;

  virtual bool hasPropertiesSection() const = 0;
  virtual void propertiesSectionTitle(QString &) const = 0;

  virtual void properties(SEntity *, QStringList &) const = 0;
  virtual void *findProperty(SEntity *, const QString &) const = 0;

  virtual void addProperty(SEntity *) const = 0;
  virtual void removeProperty(SEntity *, const QString &) const = 0;

  virtual xsize numberOfTypeSubParameters(SEntity *, void *property) const = 0;
  virtual void typeSubParameter(SEntity *, void *prop, xsize i, QString& name, QWidget *&widget) const = 0;
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

  enum
    {
    SubName,
    SubType,
    NumberOfSubTypeParameters
    };

  virtual xsize numberOfTypeParameters(SEntity *prop) const;
  virtual void typeParameter(SEntity *prop, xsize index, QString& name, QWidget *&widget) const;

  virtual bool hasPropertiesSection() const;
  virtual void propertiesSectionTitle(QString &n) const;

  virtual void properties(SEntity *, QStringList &) const;
  virtual void *findProperty(SEntity *, const QString &) const;

  virtual void addProperty(SEntity *) const;
  virtual void removeProperty(SEntity *, const QString &) const;

  virtual xsize numberOfTypeSubParameters(SEntity *, void *property) const;
  virtual void typeSubParameter(SEntity *, void *prop, xsize i, QString& name, QWidget *&widget) const;
  };

#endif // SAPARTEDITORINTERFACE_H
