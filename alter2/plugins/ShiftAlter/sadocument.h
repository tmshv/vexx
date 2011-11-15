#ifndef SDOCUMENT_H
#define SDOCUMENT_H

#include "saglobal.h"
#include "sentity.h"
#include "QWidget"
#include "sentityweakpointer.h"
#include "sbaseproperties.h"

class QMenu;

class SHIFTALTER_EXPORT SDocument : public SEntity
  {
  S_ENTITY(SDocument, SEntity, 0)

public:
  SDocument();

  StringProperty filename;
  SPropertyArray transientData;

  void save(QWidget *parent);
  void saveAs(QWidget *parent, const QString &filename="");
  };

class SDocumentEditor : public QWidget
  {
  Q_OBJECT

XProperties:
  XROProperty(SEntityTypedWeakPointer<SDocument>, document);

public:
  SDocumentEditor(SDocument *doc);

  void buildFileMenu(QMenu *menu);

private slots:
  void save();
  void saveAs();
  };

#endif // SDOCUMENT_H
