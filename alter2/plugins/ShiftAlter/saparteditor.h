#ifndef SAPARTEDITOR_H
#define SAPARTEDITOR_H

#include "saglobal.h"
#include "saparteditorinterface.h"
#include "sadocument.h"
#include "sentityweakpointer.h"
#include "XProperty"
#include "QWidget"

class SProperty;
class SPropertyContainer;

class QListWidget;

class SHIFTALTER_EXPORT SPartDocument : public SDocument
  {
  S_ENTITY(SPartDocument, SDocument, 0)

public:
  SPartDocument();

  StringProperty type;

  virtual QWidget *createEditor();
  };

class SHIFTALTER_EXPORT SPartEditor : public SDocumentEditor, public SPartEditorInterfaceFeedbacker
  {
  Q_OBJECT

XProperties:
  XReadProperty(const SPartEditorInterface *, partInterface, partInterface);
  XROProperty(SEntityWeakPointer, part);

public:
  SPartEditor(SPartDocument *holder);

private slots:
  void addProperty();
  void removeProperty();
  void selectProperty();

private:
  const SPartEditorInterface *findInterface(const QString& t);

  QLayout *buildEntitySection();
  QLayout *buildTypeParameters();
  QLayout *buildProperties();

  SPartDocument *partDocument();

  void rebuildUI();
  void rebuildTypeParameters(QWidget *widget, SEntity *ent);
  void rebuildPropertyList(QListWidget *);
  void rebuildPropertyProperties(QWidget *widget, void *property);

  void propertyNameChanged();
  void propertySubParametersChanged(SProperty *);
  void refreshAll(SProperty *);

  QListWidget *_list;
  QWidget *_main;
  QWidget *_propertyProperties;
  QWidget *_propertyPropertiesInternal;
  QWidget *_typeParameters;
  QWidget *_typeParametersInternal;

  XPropertyMember(const SPartEditorInterface *, partInterface);
  };

#endif // SAPARTEDITOR_H
