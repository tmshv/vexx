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

  virtual void newFile();
  virtual void loadFile(const QString &filename);
  virtual QWidget *createEditor();
  };

class SHIFTALTER_EXPORT SPartEditor : public SDocumentEditor, public SPartEditorInterfaceFeedbacker
  {
  Q_OBJECT

XProperties:
  XReadProperty(const SPartEditorInterface *, partInterface, partInterface);

public:
  SPartEditor(SPlugin *p, SPartDocument *holder);

private slots:
  void addProperty();
  void removeProperty();
  void selectProperty();

private:
  const SPartEditorInterface *findInterface(const QString& t);

  SEntity *part();

  QLayout *buildEntitySection();
  QLayout *buildTypeParameters();
  QLayout *buildProperties();

  SPartDocument *partDocument();

  void reloadUI();
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

  SEntityWeakPointer _part;

  XPropertyMember(const SPartEditorInterface *, partInterface);
  };

#endif // SAPARTEDITOR_H
