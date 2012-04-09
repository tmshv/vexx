#ifndef SAPARTEDITORINTERFACE_H
#define SAPARTEDITORINTERFACE_H

#include "saglobal.h"
#include "sinterface.h"
#include "QLineEdit"
#include "QComboBox"
#include "sentity.h"
#include "sentityweakpointer.h"

class SPartEditorInterfaceFeedbacker
  {
public:
  virtual void propertyNameChanged() = 0;
  virtual void propertySubParametersChanged(SProperty *) = 0;
  virtual void refreshAll(SProperty *) = 0;
  };

class SPartEditorInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(SPartEditorInterface, SPartEditorInterface)

public:
  SPartEditorInterface(bool deleteOnNoReferences) : SStaticInterfaceBase(deleteOnNoReferences) { }
  virtual ~SPartEditorInterface() { }

  virtual xsize numberOfTypeParameters(SEntity *prop) const = 0;
  virtual void typeParameter(SPartEditorInterfaceFeedbacker *, SEntity *prop, xsize index, QString& name, QWidget *&widget) const = 0;

  virtual bool hasPropertiesSection() const = 0;
  virtual void propertiesSectionTitle(QString &) const = 0;

  virtual void properties(SEntity *, QStringList &) const = 0;
  virtual void *findProperty(SEntity *, const QString &) const = 0;

  virtual void addProperty(SEntity *) const = 0;
  virtual void removeProperty(SEntity *, const QString &) const = 0;

  virtual xsize numberOfTypeSubParameters(SEntity *, void *property) const = 0;
  virtual void typeSubParameter(SPartEditorInterfaceFeedbacker *, SEntity *, void *prop, xsize i, QString& name, QWidget *&widget) const = 0;

  virtual QWidget *buildCustomEditor(SEntity *) const = 0;
  virtual QWidget *buildCustomPreview(const SEntity *, SPropertyArray *) const = 0;
  };

class PropertyNameEditor : public QLineEdit, public STreeObserver
  {
  Q_OBJECT

public:
  typedef void (SPartEditorInterfaceFeedbacker::*CallbackFn)();

XProperties:
  XProperty(SProperty *, property, setProperty)
  XProperty(CallbackFn, callback, setCallback)
  XProperty(SPartEditorInterfaceFeedbacker *, feedback, setFeedback)

public:
  PropertyNameEditor(SProperty *e, SPartEditorInterfaceFeedbacker *f=0, CallbackFn callback=0);
  virtual ~PropertyNameEditor();

  virtual void onTreeChange(const SChange *, bool back);

public slots:
  void editName();

private:
  SEntityWeakPointer _observedEntity;
  };

class PropertyTypeEditor : public QComboBox
  {
  Q_OBJECT

public:
  typedef void (SPartEditorInterfaceFeedbacker::*CallbackFn)(SProperty *);

XProperties:
  XProperty(SProperty *, property, setProperty)
  XProperty(CallbackFn, callback, setCallback)
  XProperty(SPartEditorInterfaceFeedbacker *, feedback, setFeedback)

public:
  PropertyTypeEditor(SProperty *e, const QStringList &items, SPartEditorInterfaceFeedbacker *f=0, CallbackFn callback=0);
  ~PropertyTypeEditor();

public slots:
  void editType(const QString &);
  };

class SHIFTALTER_EXPORT SDefaultPartEditorInterface : public SPartEditorInterface
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
  virtual void typeParameter(SPartEditorInterfaceFeedbacker *, SEntity *prop, xsize index, QString& name, QWidget *&widget) const;
  virtual QStringList possibleTypes() const;

  virtual bool hasPropertiesSection() const;
  virtual void propertiesSectionTitle(QString &n) const;

  virtual void properties(SEntity *, QStringList &) const;
  virtual void *findProperty(SEntity *, const QString &) const;
  virtual QStringList possiblePropertyTypes() const;

  virtual void addProperty(SEntity *) const;
  virtual void removeProperty(SEntity *, const QString &) const;

  virtual xsize numberOfTypeSubParameters(SEntity *, void *property) const;
  virtual void typeSubParameter(SPartEditorInterfaceFeedbacker *, SEntity *, void *prop, xsize i, QString& name, QWidget *&widget) const;

  virtual QWidget *buildCustomEditor(SEntity *) const { return 0; }
  virtual QWidget *buildCustomPreview(const SEntity *, SPropertyArray *) const { return 0; }
  };

#endif // SAPARTEDITORINTERFACE_H
