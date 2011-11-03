#include "saparteditorinterface.h"
#include "sentity.h"
#include "QLineEdit"
#include "QComboBox"

PropertyNameEditor::PropertyNameEditor(SProperty *e, SPartEditorInterfaceFeedbacker *feedback, CallbackFn callback)
    : _property(e), _feedback(feedback), _callback(callback)
  {
  setText(e->name());
  xAssert(e->isDynamic());
  connect(this, SIGNAL(editingFinished()), this, SLOT(editName()));

  if(property()->entity())
    {
    property()->entity()->addTreeObserver(this);
    }
  }

PropertyNameEditor::~PropertyNameEditor()
  {
  if(property()->entity())
    {
    property()->entity()->removeTreeObserver(this);
    }
  }

void PropertyNameEditor::onTreeChange(const SChange *c)
  {
  const SProperty::NameChange *n = c->castTo<SProperty::NameChange>();
  if(n && n->property() == property())
    {
    setText(n->after());
    }
  }

void PropertyNameEditor::editName()
  {
  property()->setName(text());
  if(feedback() && callback())
    {
    (feedback()->*callback())();
    }
  }

SDefaultPartEditorInterface::SDefaultPartEditorInterface() : SPartEditorInterface(true)
  {
  }

QStringList SDefaultPartEditorInterface::possibleTypes() const
  {
  const SPropertyInformation *info = SEntity::staticTypeInformation();

  QStringList list;
  foreach(const SPropertyInformation *i, STypeRegistry::types())
    {
    if(i->inheritsFromType(info))
      {
      list << i->typeName();
      }
    }

  qSort(list);
  return list;
  }

xsize SDefaultPartEditorInterface::numberOfTypeParameters(SEntity *) const
  {
  return NumberOfTypeParameters;
  }

void SDefaultPartEditorInterface::typeParameter(SEntity *e, xsize index, QString& name, QWidget *&widget) const
  {
  if(index == Name)
    {
    name = "Name";
    widget = new PropertyNameEditor(e);
    }
  else if(index == Type)
    {
    name = "Type";
    QComboBox *combo = new QComboBox;

    QStringList types = possibleTypes();
    combo->addItems(types);

    widget = combo;
    }
  else
    {
    xAssertFail();
    }
  }

bool SDefaultPartEditorInterface::hasPropertiesSection() const
  {
  return true;
  }

void SDefaultPartEditorInterface::propertiesSectionTitle(QString &n) const
  {
  n = "Properties";
  }

void SDefaultPartEditorInterface::properties(SEntity *p, QStringList &l) const
  {
  SProperty *c = p->firstChild();
  while(c)
    {
    if(c->isDynamic())
      {
      l << c->name();
      }

    c = c->nextSibling();
    }
  }

void *SDefaultPartEditorInterface::findProperty(SEntity *c, const QString &n) const
  {
  return c->findChild(n);
  }

QStringList SDefaultPartEditorInterface::possiblePropertyTypes() const
  {
  QStringList list;
  foreach(const SPropertyInformation *i, STypeRegistry::types())
    {
    list << i->typeName();
    }

  qSort(list);
  return list;
  }

void SDefaultPartEditorInterface::addProperty(SEntity *c) const
  {
  c->addProperty<SProperty>();
  }

void SDefaultPartEditorInterface::removeProperty(SEntity *c, const QString &n) const
  {
  SProperty *p = c->findChild(n);
  xAssert(p);
  if(p)
    {
    c->removeProperty(p);
    }
  }

xsize SDefaultPartEditorInterface::numberOfTypeSubParameters(SEntity *, void *) const
  {
  return NumberOfSubTypeParameters;
  }

void SDefaultPartEditorInterface::typeSubParameter(SPartEditorInterfaceFeedbacker *f, SEntity *, void *prop, xsize i, QString& name, QWidget *&widget) const
  {
  if(i == SubName)
    {
    name = "Name";
    widget = new PropertyNameEditor((SProperty*)prop, f, &SPartEditorInterfaceFeedbacker::propertyNameChanged);
    }
  else if(i == SubType)
    {
    name = "Type";
    QComboBox *combo = new QComboBox;

    QStringList types = possiblePropertyTypes();
    combo->addItems(types);

    widget = combo;
    }
  else
    {
    xAssertFail();
    }
  }
