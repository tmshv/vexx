#include "sadocument.h"
#include "QFileDialog"
#include "Serialisation/sjsonio.h"
#include "QMenu"

void SDocument::incrementRevision(const SPropertyInstanceInformation *, SPropertyContainer *c)
  {
  SDocument *doc = c->uncheckedCastTo<SDocument>();

  doc->revision = doc->revision() + 1;
  }

S_IMPLEMENT_ABSTRACT_PROPERTY(SDocument)

SPropertyInformation *SDocument::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SDocument>("SDocument");

  info->add(&SDocument::transientData, "transientData");

  info->add(&SDocument::filename, "filename");

  UnsignedIntProperty::InstanceInformation *rev = info->add(&SDocument::revision, "revision");
  rev->setCompute(incrementRevision);

  PointerArray::InstanceInformation *fCS = info->add(&SDocument::fileChangedStub, "fileChangedStub");
  fCS->setAffects(rev);

  return info;
  }

SDocument::SDocument()
  {
  }

bool SDocument::hasChanged()
  {
  return _checkpoint == revision();
  }

void SDocument::newFile()
  {
  SBlock b(database());
  filename = "";

  fileChangedStub.clear();
  children.clear();
  transientData.clear();

  _checkpoint = 0;
  revision = 0;
  }

void SDocument::loadFile(const QString &f)
  {
  QFile file(f);
  if(file.open(QIODevice::ReadOnly))
    {
    SJSONLoader loader;

    SBlock b(database());

    fileChangedStub.clear();
    children.clear();
    transientData.clear();

    loader.readFromDevice(&file, &children);
    }
  else
    {
    qWarning() << "Failed to open file for reading '" << filename() << "'";
    return;
    }

  filename = f;
  _checkpoint = revision();
  }

void SDocument::saveFile(const QString &f)
  {
  QFile file(f);
  if(file.open(QIODevice::WriteOnly))
    {
    SJSONSaver saver;
    saver.setAutoWhitespace(true);

    saver.writeToDevice(&file, &children, false);
    }
  else
    {
    qWarning() << "Failed to open file for writing '" << filename() << "'";
    return;
    }

  filename = f;
  _checkpoint = revision();
  }


SDocumentEditor::SDocumentEditor(SDocument *doc) : _document(doc)
  {
  }

void SDocumentEditor::buildFileMenu(QMenu *menu)
  {
  menu->addAction("New", this, SLOT(newFile()));
  menu->addAction("Open", this, SLOT(loadFile()));
  QAction *save = menu->addAction("Save", this, SLOT(saveFile()));
  menu->addAction("Save As", this, SLOT(saveAsFile()));

  save->setObjectName("Save");

  connect(menu, SIGNAL(aboutToShow()), this, SLOT(enableMenu()));
  }

void SDocumentEditor::reloadUI()
  {
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
  QString fileStr = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), "", QObject::tr("Json Data (*.json)"));

  document()->loadFile(fileStr);

  reloadUI();
  }

void SDocumentEditor::saveFile()
  {
  document()->saveFile(document()->filename());
  }

void SDocumentEditor::saveAsFile()
  {
  QString fileStr = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), "", QObject::tr("Json Data (*.json)"));

  document()->saveFile(fileStr);
  }

