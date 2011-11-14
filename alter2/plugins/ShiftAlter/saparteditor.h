#ifndef SAPARTEDITOR_H
#define SAPARTEDITOR_H

#include "saglobal.h"
#include "saparteditorinterface.h"
#include "sentityweakpointer.h"
#include "XProperty"
#include "QWidget"

class SProperty;
class SPropertyContainer;

class QListWidget;

class PartEditorHolder : public SEntity
  {
  S_PROPERTY(PartEditorHolder, SEntity, 0)
public:
  PartEditorHolder();

  SEntity previewEntities;
  };

class SHIFTALTER_EXPORT SPartEditor : public QWidget, public SPartEditorInterfaceFeedbacker
  {
  Q_OBJECT

XProperties:
  XReadProperty(const SPartEditorInterface *, partInterface, partInterface);
  XROProperty(SEntityWeakPointer, holder);

public:
  static SPartEditor *editNewPart(const QString &type, const QString &name, SEntity *parent);
  //static SPartEditor *editPart(const QString &type, SProperty *parent);

private slots:
  void addProperty();
  void removeProperty();
  void selectProperty();
  void save();

private:
  SPartEditor(const QString &type, PartEditorHolder *holder, SEntity *prop);

  const SPartEditorInterface *findInterface(const QString& t);

  QLayout *buildEntitySection();
  QLayout *buildTypeParameters();
  QLayout *buildProperties();


  void rebuildTypeParameters(QWidget *widget, SEntity *ent);
  void rebuildPropertyList(QListWidget *);
  void rebuildPropertyProperties(QWidget *widget, void *property);

  void propertyNameChanged();
  void propertySubParametersChanged(SProperty *);
  void refreshAll(SProperty *);

  QListWidget *_list;
  QWidget *_propertyProperties;
  QWidget *_propertyPropertiesInternal;
  QWidget *_typeParameters;
  QWidget *_typeParametersInternal;

  XPropertyMember(const SPartEditorInterface *, partInterface);
  };

#endif // SAPARTEDITOR_H
