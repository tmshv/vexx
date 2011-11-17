#ifndef SDOCUMENT_H
#define SDOCUMENT_H

#include "saglobal.h"
#include "sentity.h"
#include "QWidget"
#include "sentityweakpointer.h"
#include "sbaseproperties.h"
#include "sbasepointerproperties.h"

class QMenu;

class SHIFTALTER_EXPORT SDocument : public SEntity
  {
  S_ABSTRACT_ENTITY(SDocument, SEntity, 0)

public:
  SDocument();

  StringProperty filename;
  SPropertyArray transientData;

  PointerArray fileChangedStub;

  virtual QWidget *createEditor() = 0;

  bool hasChanged();

  virtual void newFile();
  virtual void loadFile(const QString &filename);
  virtual void saveFile(const QString &filename="");

private:
  static void incrementRevision(const SPropertyInstanceInformation *info, SPropertyContainer *c);
  UnsignedIntProperty revision;
  xuint32 _checkpoint;
  };

class SHIFTALTER_EXPORT SDocumentEditor : public QWidget
  {
  Q_OBJECT

XProperties:
  XROProperty(SEntityTypedWeakPointer<SDocument>, document);

public:
  SDocumentEditor(SDocument *doc);

  void buildFileMenu(QMenu *menu);

  virtual void reloadUI();

private slots:
  void newFile();
  void loadFile();
  void saveFile();
  void saveAsFile();

  void enableMenu();
  };

#endif // SDOCUMENT_H
