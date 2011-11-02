#include "saparteditor.h"
#include "styperegistry.h"
#include "sentity.h"
#include "QFormLayout"
#include "QLabel"
#include "QListWidget"
#include "QPushButton"
#include "saparteditorinterface.h"

SPartEditor::SPartEditor(const QString &type, SEntity *prop) : _property(prop), _list(0)
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
