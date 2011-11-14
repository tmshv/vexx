#include "saparteditor.h"
#include "styperegistry.h"
#include "sentity.h"
#include "QFormLayout"
#include "QLabel"
#include "QGroupBox"
#include "QListWidget"
#include "QMenuBar"
#include "QPushButton"
#include "QFileDialog"
#include "saparteditorinterface.h"
#include "Serialisation/sjsonio.h"

S_IMPLEMENT_ABSTRACT_PROPERTY(PartEditorHolder)

SPropertyInformation *PartEditorHolder::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<PartEditorHolder>("PartEditorHolder");

  info->add(&PartEditorHolder::previewEntities, "previewEntities");

  return info;
  }

PartEditorHolder::PartEditorHolder()
  {
  }

SPartEditor::SPartEditor(const QString &type, PartEditorHolder *h, SEntity *prop) : _holder(h),
    _list(0), _propertyProperties(0), _propertyPropertiesInternal(0),
    _typeParameters(0), _typeParametersInternal(0)
  {
  QVBoxLayout *menuLayout(new QVBoxLayout(this));
  menuLayout->setContentsMargins(0, 0, 0, 0);
  QMenuBar *menu(new QMenuBar);
  menuLayout->addWidget(menu);

  QMenu *file(menu->addMenu("File"));
  file->addAction("Save", this, SLOT(save()));

  QWidget *main(new QWidget);
  menuLayout->addWidget(main);
  _partInterface = findInterface(type);

  QHBoxLayout *mainLayout(new QHBoxLayout(main));

  QLayout *entitySection = buildEntitySection();
  mainLayout->addLayout(entitySection);

  QWidget *customEditor = partInterface()->buildCustomEditor(holder());
  if(customEditor)
    {
    mainLayout->addWidget(customEditor);
    }

  SEntity *previewContainer = &holder()->castTo<PartEditorHolder>()->previewEntities;

  QWidget *customPreview = partInterface()->buildCustomPreview(holder(), previewContainer);
  if(customPreview)
    {
    mainLayout->addWidget(customPreview);
    }
  }

QLayout *SPartEditor::buildEntitySection()
  {
  QVBoxLayout *layout(new QVBoxLayout);

  _typeParameters = new QWidget;
  layout->addWidget(_typeParameters);
  rebuildTypeParameters(_typeParameters, holder());

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

QLayout *SPartEditor::buildTypeParameters()
  {
  QFormLayout *layout(new QFormLayout());
  layout->setContentsMargins(0, 0, 0, 0);

  QString name;
  QWidget *w;
  for(xsize i=0, s=partInterface()->numberOfTypeParameters(holder()); i<s; ++i)
    {
    name.clear();
    w = 0;

    partInterface()->typeParameter(this, holder(), i, name, w);
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
  partInterface()->properties(holder(), l);
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
  _holder = newEnt->castTo<SEntity>();
  xAssert(holder());

  rebuildTypeParameters(_typeParameters, holder());
  rebuildPropertyList(_list);
  selectProperty();
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
    for(xsize i=0, s=partInterface()->numberOfTypeSubParameters(holder(), prop); i<s; ++i)
      {
      name.clear();
      w = 0;

      partInterface()->typeSubParameter(this, holder(), prop, i, name, w);
      xAssert(!name.isEmpty());
      xAssert(w);

      layout->addRow(name, w);
      }
    }
  }

SPartEditor *SPartEditor::editNewPart(const QString &type, const QString &name, SEntity *parent)
  {
  const SPropertyInformation *info = STypeRegistry::findType(type);
  xAssert(info);

  if(info)
    {
    PartEditorHolder *holder = parent->addChild<PartEditorHolder>(name);
    xAssert(holder);

    SEntity *p = holder->addChild(info, name)->castTo<SEntity>();
    xAssert(p);

    return new SPartEditor(type, holder, p);
    }

  return 0;
  }

void SPartEditor::addProperty()
  {
  partInterface()->addProperty(holder());
  rebuildPropertyList(_list);
  }

void SPartEditor::removeProperty()
  {
  QList<QListWidgetItem *> selection = _list->selectedItems();

  foreach(const QListWidgetItem *i, selection)
    {
    QString n = i->text();
    partInterface()->removeProperty(holder(), n);
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

    void *data = partInterface()->findProperty(holder(), name);

    rebuildPropertyProperties(_propertyProperties, data);
    }
  else
    {
    rebuildPropertyProperties(_propertyProperties, 0);
    }
  }

void SPartEditor::save()
  {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Json Data (*.json)"));

  QFile file(fileName);
  if(file.open(QIODevice::WriteOnly))
    {
    SJSONSaver saver;
    saver.setAutoWhitespace(true);

    saver.writeToDevice(&file, &holder()->children, false);
    }
  else
    {
    qWarning() << "Failed to open file for writing '" << fileName << "'";
    }
  }
