#ifndef SAPARTEDITOR_H
#define SAPARTEDITOR_H

#include "saglobal.h"
#include "saparteditorinterface.h"
#include "XProperty"
#include "QWidget"

class SProperty;
class SPropertyContainer;

class QListWidget;

class SHIFTALTER_EXPORT SPartEditor : public QWidget, public SPartEditorInterfaceFeedbacker
  {
  Q_OBJECT

XProperties:
  XReadProperty(const SPartEditorInterface *, partInterface, partInterface);
  XROProperty(SEntity *, property);

public:
  static SPartEditor *editNewPart(const QString &type, const QString &name, SEntity *parent);
  //static SPartEditor *editPart(const QString &type, SProperty *parent);

private slots:
  void addProperty();
  void removeProperty();
  void selectProperty();

private:
  SPartEditor(const QString &type, SEntity *prop);

  const SPartEditorInterface *findInterface(const QString& t);

  QLayout *buildEntitySection();
  QLayout *buildTypeParameters();
  QLayout *buildProperties();
  void rebuildPropertyList(QListWidget *);
  void rebuildPropertyProperties(QWidget *widget, void *property);

  void propertyNameChanged();
  void propertySubParametersChanged();

  QListWidget *_list;
  QWidget *_propertyProperties;
  QWidget *_propertyPropertiesInternal;

  XPropertyMember(const SPartEditorInterface *, partInterface);
  };

#endif // SAPARTEDITOR_H
