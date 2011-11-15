#include "sadocument.h"
#include "QFileDialog"
#include "Serialisation/sjsonio.h"
#include "QMenu"

void SDocument::incrementRevision(const SPropertyInformation *info, SPropertyContainer *c)
  {
  SDocument *doc = c->uncheckedCastTo<SDocument>();

  doc->revision = doc->revision() + 1;
  }

S_IMPLEMENT_PROPERTY(SDocument)

SPropertyInformation *SDocument::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SDocument>("SDocument");

  info->add(&SDocument::transientData, "transientData");

  info->add(&SDocument::filename, "filename");

  UnsignedIntProperty::InstanceInformation *rev = info->add(&SDocument::revision, "revision");
  rev->setCompute(incrementRevision);

  SProperty::InstanceInformation *fCS = info->add(&SDocument::fileChangedStub);
  fCS->setAffects(rev);

  return info;
  }

SDocument::SDocument()
  {
  }

void SDocument::saveAs(QWidget *parent, const QString &inFile)
  {
  QString fileStr = inFile;
  if(fileStr.isEmpty())
    {
    fileStr = QFileDialog::getSaveFileName(parent, QObject::tr("Save File"), "", QObject::tr("Json Data (*.json)"));
    }

  filename = fileStr;
  if(!filename().isEmpty())
    {
    save(parent);
    }
  else
    {
    qWarning() << "Failed to save file empty file path passed.";
    }
  }

bool SDocument::hasChanged()
  {
  return _checkpoint == revision();
  }

void SDocument::save(QWidget *parent)
  {
  if(filename().isEmpty())
    {
    saveAs(parent);
    return;
    }

  QFile file(filename());
  if(file.open(QIODevice::WriteOnly))
    {
    SJSONSaver saver;
    saver.setAutoWhitespace(true);

    saver.writeToDevice(&file, &children, false);
    }
  else
    {
    qWarning() << "Failed to open file for writing '" << filename() << "'";
    }

  _checkpoint = revision();
  }


SDocumentEditor::SDocumentEditor(SDocument *doc) : _document(doc)
  {
  }

void SDocumentEditor::buildFileMenu(QMenu *menu)
  {
  menu->addAction("Save", this, SLOT(save()));
  menu->addAction("Save As", this, SLOT(saveAs()));


  }

void SDocumentEditor::saveAs()
  {
  document()->saveAs(this);
  }

void SDocumentEditor::save()
  {
  document()->save(this);
  }
