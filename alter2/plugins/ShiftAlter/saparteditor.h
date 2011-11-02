#ifndef SAPARTEDITOR_H
#define SAPARTEDITOR_H

#include "saglobal.h"
#include "XProperty"
#include "QWidget"

class SProperty;
class SPropertyContainer;
class SPartEditorInterface;

class QListWidget;

class SHIFTALTER_EXPORT SPartEditor : public QWidget
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

  QLayout *buildTypeParameters();
  QLayout *buildProperties();
  void rebuildPropertyList(QListWidget *);
  void rebuildPropertyProperties(QWidget *widget, void *property);

  QListWidget *_list;
  QWidget *_propertyProperties;
  QWidget *_propertyPropertiesInternal;

  XPropertyMember(const SPartEditorInterface *, partInterface);
  };

#endif // SAPARTEDITOR_H
