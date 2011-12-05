#ifndef SDOCUMENT_H
#define SDOCUMENT_H

#include "saglobal.h"
#include "sentity.h"
#include "QWidget"
#include "sentityweakpointer.h"
#include "sbaseproperties.h"
#include "sbasepointerproperties.h"
#include "XShared"
#include "QVector"

class QMenu;

class SExportableInterface : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(SExportableInterface, SExportableInterface)

public:
  class Exporter : public XSharedData
    {
  public:
    virtual bool exportFile(const QString &file, SProperty *) const = 0;
    virtual QString exporterName() const = 0;
    virtual QString exporterFileType() const = 0;
    };

  SExportableInterface() : SStaticInterfaceBase(true) { }

  void addExporter(const Exporter *e) const
    {
    _exporters << XConstSharedPointer<Exporter>(e);
    }

  static void addExporter(const SPropertyInformation *i, const Exporter *e)
    {
    xAssert(i);
    const SExportableInterface *ifc = static_cast<const SExportableInterface *>(i->interfaceFactory(SExportableInterface::InterfaceTypeId));
    if(!ifc)
      {
      SExportableInterface *mIfc = new SExportableInterface();
      i->addStaticInterface(mIfc);

      ifc = mIfc;
      }

    ifc->addExporter(e);
    }

  const QVector <XConstSharedPointer<Exporter> > &exporters() const { return _exporters; }

  template <typename T> static void registerExporter(const Exporter *e)
    {
    const SPropertyInformation *i = T::staticTypeInformation();
    addExporter(i, e);
    }

private:
  mutable QVector <XConstSharedPointer<Exporter> > _exporters;
  };

class SHIFTALTER_EXPORT SDocument : public SEntity
  {
  S_ABSTRACT_ENTITY(SDocument, SEntity, 0)

public:
  SDocument();

  StringProperty filename;
  SPropertyArray transientData;

  PointerArray fileChangedStub;

  virtual QWidget *createEditor() = 0;

  void setCheckpoint();
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
  void exportFile();

  void enableMenu();
  };

#endif // SDOCUMENT_H
