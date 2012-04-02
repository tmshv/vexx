#include "saparteditor.h"
#include "styperegistry.h"
#include "sentity.h"
#include "QFormLayout"
#include "QLabel"
#include "QGroupBox"
#include "QListWidget"
#include "QMenuBar"
#include "QPushButton"
#include "saparteditorinterface.h"

S_IMPLEMENT_PROPERTY(SPartDocument)

void SPartDocument::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&SPartDocument::type, "type");
    }
  }

SPartDocument::SPartDocument()
  {
  }

void SPartDocument::newFile()
  {
  SDocument::newFile();

  const SPropertyInformation *info = STypeRegistry::findType(type());
  xAssert(info);

  if(info)
    {
    xuint32 i = 1;
    QString name = info->typeName();
    while(STypeRegistry::findType(name))
      {
      name = info->typeName() + QString::number(i);
      i++;
      }

    SProperty *prop = addChild(info, name);
    fileChangedStub.addPointer(prop);
    }
  }

void SPartDocument::loadFile(const QString &filename)
  {
  SDocument::loadFile(filename);

  SProperty *prop = firstChild();
  xAssert(prop);
  if(prop)
    {
    fileChangedStub.addPointer(prop);
    }
  }

QWidget *SPartDocument::createEditor()
  {
  return new SPartEditor(plugin(), this);
  }

SPartEditor::SPartEditor(SPlugin *p, SPartDocument *holder) : SDocumentEditor(p, holder),
    _main(0), _list(0), _propertyProperties(0), _propertyPropertiesInternal(0),
    _typeParameters(0), _typeParametersInternal(0)
  {
  xAssert(!partDocument()->type().isEmpty());
  _partInterface = findInterface(partDocument()->type());
  xAssert(_partInterface)

  QVBoxLayout *menuLayout(new QVBoxLayout(this));
  menuLayout->setContentsMargins(0, 0, 0, 0);
  QMenuBar *menu(new QMenuBar);
  menuLayout->addWidget(menu);

  QMenu *file(menu->addMenu("File"));
  buildFileMenu(file);

  reloadUI();
  }

SPartDocument *SPartEditor::partDocument()
  {
  if(document().isValid())
    {
    return document()->uncheckedCastTo<SPartDocument>();
    }
  return 0;
  }

QLayout *SPartEditor::buildEntitySection()
  {
  QVBoxLayout *layout(new QVBoxLayout);

  _typeParameters = new QWidget;
  layout->addWidget(_typeParameters);
  rebuildTypeParameters(_typeParameters, part());

  if(partInterface()->hasPropertiesSection())
    {
    QLayout *l2 = buildProperties();
    layout->addLayout(l2);
    }
  return layout;
  }

const SPartEditorInterface *SPartEditor::findInterface(const QString& t)
  {
  const SPropertyInformation *info = STypeRegistry::findType(t);
  xAssert(info);

  const SInterfaceBaseFactory *factory = info->interfaceFactory(SPartEditorInterface::InterfaceTypeId);
  xAssert(factory);

  return static_cast<const SPartEditorInterface*>(factory);
  }

SEntity *SPartEditor::part()
  {
  return _part;
  }

QLayout *SPartEditor::buildTypeParameters()
  {
  QFormLayout *layout(new QFormLayout());
  layout->setContentsMargins(0, 0, 0, 0);

  QString name;
  QWidget *w;
  for(xsize i=0, s=partInterface()->numberOfTypeParameters(part()); i<s; ++i)
    {
    name.clear();
    w = 0;

    partInterface()->typeParameter(this, part(), i, name, w);
    xAssert(!name.isEmpty());
    xAssert(w);

    layout->addRow(name, w);
    }

  return layout;
  }

QLayout *SPartEditor::buildProperties()
  {
  QVBoxLayout *layout(new QVBoxLayout());

  QString t;
  partInterface()->propertiesSectionTitle(t);
  QLabel *label = new QLabel("<b>" + t + "<\b>");

  QPushButton *add(new QPushButton("Add"));
  connect(add, SIGNAL(clicked()), this, SLOT(addProperty()));
  QPushButton *remove(new QPushButton("Remove"));
  connect(remove, SIGNAL(clicked()), this, SLOT(removeProperty()));

  QHBoxLayout *headerLayout(new QHBoxLayout);
  layout->addLayout(headerLayout);

  headerLayout->addWidget(label);
  headerLayout->addWidget(add);
  headerLayout->addWidget(remove);

  _list = new QListWidget;
  layout->addWidget(_list);
  rebuildPropertyList(_list);

  connect(_list, SIGNAL(clicked(QModelIndex)), this, SLOT(selectProperty()));

  _propertyProperties = new QWidget;
  layout->addWidget(_propertyProperties);

  return layout;
  }

void SPartEditor::rebuildPropertyList(QListWidget *list)
  {
  list->clear();

  QStringList l;
  partInterface()->properties(part(), l);
  foreach(const QString &n, l)
    {
    list->addItem(n);
    }
  }

void SPartEditor::propertyNameChanged()
  {
  rebuildPropertyList(_list);
  }

void SPartEditor::propertySubParametersChanged(SProperty *)
  {
  selectProperty();
  }

void SPartEditor::refreshAll(SProperty *newEnt)
  {
  _part = newEnt->castTo<SEntity>();
  xAssert(part());

  rebuildTypeParameters(_typeParameters, part());
  rebuildPropertyList(_list);
  selectProperty();
  }


void SPartEditor::reloadUI()
  {
  if(_main)
    {
    _main->deleteLater();
    }

  _part = document()->children.firstChild<SEntity>();
  xAssert(_part);

  _main = new QWidget();
  layout()->addWidget(_main);

  QHBoxLayout *mainLayout(new QHBoxLayout(_main));

  QLayout *entitySection = buildEntitySection();
  mainLayout->addLayout(entitySection);

  QWidget *customEditor = partInterface()->buildCustomEditor(part());
  if(customEditor)
    {
    mainLayout->addWidget(customEditor);
    }

  SPropertyArray *previewContainer = &document()->transientData;

  QWidget *customPreview = partInterface()->buildCustomPreview(part(), previewContainer);
  if(customPreview)
    {
    mainLayout->addWidget(customPreview);
    }
  }

void SPartEditor::rebuildTypeParameters(QWidget *widget, SEntity *ent)
  {
  // delete old internal
  if(_typeParametersInternal)
    {
    // delete old layout
    delete widget->layout();
    widget->setLayout(0);

    _typeParametersInternal->deleteLater();
    _typeParametersInternal = 0;
    }

  if(ent)
    {
    // setup new internal
    QVBoxLayout *internalLayout(new QVBoxLayout(widget));
    internalLayout->setContentsMargins(0, 0, 0, 0);

    _typeParametersInternal = new QWidget;
    internalLayout->addWidget(_typeParametersInternal);

    QLayout *l = buildTypeParameters();
    _typeParametersInternal->setLayout(l);
    }
  }

void SPartEditor::rebuildPropertyProperties(QWidget *widget, void *prop)
  {
  // delete old internal
  if(_propertyPropertiesInternal)
    {
    // delete old layout
    delete widget->layout();
    widget->setLayout(0);

    _propertyPropertiesInternal->deleteLater();
    _propertyPropertiesInternal = 0;
    }


  if(prop)
    {
    // setup new internal
    QVBoxLayout *internalLayout(new QVBoxLayout(widget));
    internalLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox *groupBox = new QGroupBox;
    groupBox->setTitle("Property Properties");

    _propertyPropertiesInternal = groupBox;
    internalLayout->addWidget(_propertyPropertiesInternal);

    QFormLayout *layout(new QFormLayout(_propertyPropertiesInternal));
    //layout->setContentsMargins(0, 0, 0, 0);

    QString name;
    QWidget *w;
    for(xsize i=0, s=partInterface()->numberOfTypeSubParameters(part(), prop); i<s; ++i)
      {
      name.clear();
      w = 0;

      partInterface()->typeSubParameter(this, part(), prop, i, name, w);
      xAssert(!name.isEmpty());

      if(!w)
        {
        qWarning() << "Couldn't create sub parameter widget for" << name;
        continue;
        }

      layout->addRow(name, w);
      }
    }
  }

void SPartEditor::addProperty()
  {
  partInterface()->addProperty(part());
  rebuildPropertyList(_list);
  }

void SPartEditor::removeProperty()
  {
  QList<QListWidgetItem *> selection = _list->selectedItems();

  foreach(const QListWidgetItem *i, selection)
    {
    QString n = i->text();
    partInterface()->removeProperty(part(), n);
    }
  rebuildPropertyList(_list);
  selectProperty();
  }

void SPartEditor::selectProperty()
  {
  QList<QListWidgetItem *> selection = _list->selectedItems();

  if(selection.size() > 0)
    {
    xAssert(selection.size() == 1);
    QString name = selection[0]->text();

    void *data = partInterface()->findProperty(part(), name);

    rebuildPropertyProperties(_propertyProperties, data);
    }
  else
    {
    rebuildPropertyProperties(_propertyProperties, 0);
    }
  }
