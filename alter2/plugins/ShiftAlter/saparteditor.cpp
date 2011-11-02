#include "saparteditor.h"
#include "styperegistry.h"
#include "sentity.h"
#include "QFormLayout"
#include "QLabel"
#include "QListWidget"
#include "QPushButton"
#include "saparteditorinterface.h"

SPartEditor::SPartEditor(const QString &type, SEntity *prop) : _property(prop),
    _list(0), _propertyProperties(0), _propertyPropertiesInternal(0)
  {
  _partInterface = findInterface(type);

  QVBoxLayout *layout(new QVBoxLayout(this));

  QLayout *l = buildTypeParameters();
  layout->addLayout(l);

  if(partInterface()->hasPropertiesSection())
    {
    QLayout *l2 = buildProperties();
    layout->addLayout(l2);
    }
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

  QString name;
  QWidget *w;
  for(xsize i=0, s=partInterface()->numberOfTypeParameters(property()); i<s; ++i)
    {
    name.clear();
    w = 0;

    partInterface()->typeParameter(property(), i, name, w);
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
  QLabel *label = new QLabel("<b>" + t + "</b>");

  QPushButton *add(new QPushButton("Add"));
  connect(add, SIGNAL(clicked()), this, SLOT(addProperty()));
  QPushButton *remove(new QPushButton("Remove"));
  connect(remove, SIGNAL(clicked()), this, SLOT(removeProperty()));

  QHBoxLayout *headerLayout(new QHBoxLayout);
  layout->addLayout(headerLayout);

  headerLayout->addWidget(label);
  headerLayout->addWidget(add);
  headerLayout->addWidget(remove);

  layout->addWidget(label);

  _list = new QListWidget;
  _list->setSelectionMode(QAbstractItemView::MultiSelection);
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
  partInterface()->properties(property(), l);
  foreach(const QString &n, l)
    {
    list->addItem(n);
    }
  }

void SPartEditor::rebuildPropertyProperties(QWidget *widget, void *prop)
  {
  // delete old internal
  if(_propertyPropertiesInternal)
    {
    _propertyPropertiesInternal->deleteLater();
    }

  // delete old layout
  delete widget->layout();

  // setup new internal
  QVBoxLayout *internalLayout(new QVBoxLayout(widget));
  internalLayout->setContentsMargins(0, 0, 0, 0);
  _propertyPropertiesInternal = new QWidget;
  internalLayout->addWidget(_propertyPropertiesInternal);

  QFormLayout *layout(new QFormLayout(widget));

  QString name;
  QWidget *w;
  for(xsize i=0, s=partInterface()->numberOfTypeSubParameters(property(), prop); i<s; ++i)
    {
    name.clear();
    w = 0;

    partInterface()->typeSubParameter(property(), prop, i, name, w);
    xAssert(!name.isEmpty());
    xAssert(w);

    layout->addRow(name, w);
    }
  }

SPartEditor *SPartEditor::editNewPart(const QString &type, const QString &name, SEntity *parent)
  {
  const SPropertyInformation *info = STypeRegistry::findType(type);
  xAssert(info);

  if(info)
    {
    SEntity *p = parent->addChild(info, name)->castTo<SEntity>();

    return new SPartEditor(type, p);
    }

  return 0;
  }

void SPartEditor::addProperty()
  {
  partInterface()->addProperty(property());
  rebuildPropertyList(_list);
  }

void SPartEditor::removeProperty()
  {
  QList<QListWidgetItem *> selection = _list->selectedItems();

  foreach(const QListWidgetItem *i, selection)
    {
    QString n = i->text();
    partInterface()->removeProperty(property(), n);
    }
  rebuildPropertyList(_list);
  }

void SPartEditor::selectProperty()
  {
  QList<QListWidgetItem *> selection = _list->selectedItems();

  if(selection.size() > 0)
    {
    xAssert(selection.size() == 1);
    QString name = selection[0]->text();

    void *data = partInterface()->findProperty(property(), name);

    rebuildPropertyProperties(_propertyProperties, data);
    }
  else
    {
    rebuildPropertyProperties(_propertyProperties, 0);
    }
  }
