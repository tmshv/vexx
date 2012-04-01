#include "sadocument.h"
#include "QFileDialog"
#include "Serialisation/sjsonio.h"
#include "QMenu"
#include "splugin.h"
#include "XQObjectWrapper.h"

void SDocument::incrementRevision(const SPropertyInstanceInformation *, SDocument *doc)
  {
  doc->revision = doc->revision() + 1;
  }

S_IMPLEMENT_ABSTRACT_PROPERTY(SDocument)

void SDocument::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    SPropertyArray::InstanceInformation *transientInst = info->add(&SDocument::transientData, "transientData");
    transientInst->setMode(SPropertyInstanceInformation::Internal);

    StringProperty::InstanceInformation *filenameInst = info->add(&SDocument::filename, "filename");
    filenameInst->setMode(SPropertyInstanceInformation::Internal);

    UnsignedIntProperty::InstanceInformation *rev = info->add(&SDocument::revision, "revision");
    rev->setCompute(incrementRevision);
    rev->setMode(SPropertyInstanceInformation::Internal);

    PointerArray::InstanceInformation *fCS = info->add(&SDocument::fileChangedStub, "fileChangedStub");
    fCS->setAffects(rev);
    fCS->setMode(SPropertyInstanceInformation::Internal);
    }

  if(data.registerInterfaces)
    {
    info->addInheritedInterface<SDocument, SHandler>();

    XInterface<SDocument> *api = info->apiInterface<SDocument>();

    api->addMethod<void(), &SDocument::newFile>("newFile");
    api->addMethod<void(const QString &), &SDocument::loadFile>("loadFile");
    api->addMethod<void(const QString &), &SDocument::saveFile>("saveFile");

    api->addMethod<QWidget *(), &SDocument::createEditor>("createEditor");
    }
  }

SDocument::SDocument()
  {
  }

void SDocument::setCheckpoint()
  {
  _checkpoint = revision();
  }

bool SDocument::hasChanged()
  {
  xAssert(revision() >= _checkpoint);
  return _checkpoint != revision();
  }

void SDocument::newFile()
  {
  SBlock b(handler());

  fileChangedStub.clear();
  children.clear();
  transientData.clear();

  filename = "";
  _checkpoint = 0;
  revision = 0;
  }

void SDocument::loadFile(const QString &f)
  {
  QFile file(f);
  if(!file.open(QIODevice::ReadOnly))
    {
    qWarning() << "Failed to open file for reading '" << filename() << "'";
    return;
    }

  SJSONLoader loader;

  SBlock b(handler());

  fileChangedStub.clear();
  children.clear();
  transientData.clear();

  loader.readFromDevice(&file, &children);

  filename = f;
  _checkpoint = 0;
  revision = 0;
  }

void SDocument::saveFile(const QString &f)
  {
  QFile file(f);
  if(!file.open(QIODevice::WriteOnly))
    {
    qWarning() << "Failed to open file for writing '" << filename() << "'";
    return;
    }

   SJSONSaver saver;
   saver.setAutoWhitespace(true);

   saver.writeToDevice(&file, &children, false);

  filename = f;
  setCheckpoint();
  }


SDocumentEditor::SDocumentEditor(SPlugin *p, SDocument *doc) : _document(doc), _plugin(p)
  {
  }

void SDocumentEditor::buildFileMenu(QMenu *menu)
  {
  menu->addAction("New", this, SLOT(newFile()), QKeySequence::New);
  menu->addAction("Open", this, SLOT(loadFile()), QKeySequence::Open);
  QAction *save = menu->addAction("Save", this, SLOT(saveFile()), QKeySequence::Save);
  menu->addAction("Save As", this, SLOT(saveAsFile()), QKeySequence::SaveAs);

  save->setObjectName("Save");

  connect(menu, SIGNAL(aboutToShow()), this, SLOT(enableMenu()));

  menu->addSeparator();

  QMenu *exportMenu = menu->addMenu("Export");

  QMap <QString, const void *> exporters;

  xAssert(_document.isValid());
  const SPropertyInformation *info = _document->typeInformation();
  const SExportableInterface *ifc = 0;
  while(info)
    {
    const SExportableInterface *newIfc = static_cast<const SExportableInterface*>(info->interfaceFactory(SExportableInterface::InterfaceTypeId));
    if(newIfc != ifc)
      {
      ifc = newIfc;
      if(ifc)
        {
        foreach(const XConstSharedPointer<SExportableInterface::Exporter> &e, ifc->exporters())
          {
          QString name = e->exporterName();
          xAssert(!exporters.contains(name));
          exporters.insert(name, e.constPtr());
          }
        }
      }

    info = info->parentTypeInformation();
    }

  foreach(const QString &n, exporters.keys())
    {
    QAction *a = exportMenu->addAction(n, this, SLOT(exportFile()));

    const void *v = exporters[n];
    a->setData((quint64)v);
    }
  }

void SDocumentEditor::reloadUI()
  {
  }

void SDocumentEditor::setLastUsedDirectory(const QString &dir)
  {
  _plugin->setSetting<QString>("document", "lastUsedDirectory", dir);
  }

void SDocumentEditor::setLastUsedDirectoryFromFile(const QString &fName)
  {
  QFileInfo file(fName);
  QDir dir = file.absoluteDir();
  _plugin->setSetting<QString>("document", "lastUsedDirectory", dir.absolutePath());
  }

QString SDocumentEditor::lastUsedDirectory() const
  {
  return _plugin->setting<QString>("document", "lastUsedDirectory");
  }

void SDocumentEditor::enableMenu()
  {
  QObject *s = sender();
  if(!s)
    {
    return;
    }

  QMenu *m = qobject_cast<QMenu*>(s);
  if(!m)
    {
    return;
    }

  bool canSave = document()->hasChanged();


  QAction *save = m->findChild<QAction*>("Save");
  if(save)
    {
    save->setEnabled(canSave);
    }
  }

void SDocumentEditor::newFile()
  {
  document()->newFile();

  reloadUI();
  }

void SDocumentEditor::loadFile()
  {
  QString fileStr = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), lastUsedDirectory(), QObject::tr("Json Data (*.json)"));

  setLastUsedDirectoryFromFile(fileStr);

  document()->loadFile(fileStr);

  reloadUI();
  }

void SDocumentEditor::saveFile()
  {
  document()->saveFile(document()->filename());
  }

void SDocumentEditor::exportFile()
  {
  QAction *sen = qobject_cast<QAction*>(sender());
  xAssert(sen);

  const void *data = (const void *)sen->data().value<quint64>();
  xAssert(data);

  const SPropertyInformation *info = _document->typeInformation();
  const SExportableInterface *ifc = 0;
  while(info)
    {
    const SExportableInterface *newIfc = static_cast<const SExportableInterface*>(info->interfaceFactory(SExportableInterface::InterfaceTypeId));
    if(newIfc != ifc)
      {
      ifc = newIfc;
      if(ifc)
        {
        foreach(const XConstSharedPointer<SExportableInterface::Exporter> &e, ifc->exporters())
          {
          if(data == e.constPtr())
            {
            QString fileStr = QFileDialog::getSaveFileName(this, QObject::tr("Export File"), lastUsedDirectory(), e->exporterFileType());

            setLastUsedDirectoryFromFile(fileStr);

            e->exportFile(fileStr, _document);

            return;
            }
          }
        }
      }

    info = info->parentTypeInformation();
    }
  }

void SDocumentEditor::saveAsFile()
  {
  QString fileStr = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), lastUsedDirectory(), QObject::tr("Json Data (*.json)"));

  setLastUsedDirectoryFromFile(fileStr);

  document()->saveFile(fileStr);
  }

