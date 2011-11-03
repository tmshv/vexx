#include "saparteditorinterface.h"
#include "sentity.h"
#include "QLineEdit"
#include "QComboBox"

PropertyNameEditor::PropertyNameEditor(SProperty *e) : _property(e)
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
  }

SDefaultPartEditorInterface::SDefaultPartEditorInterface() : SPartEditorInterface(true)
  {
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
    widget = new QComboBox;
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

void SDefaultPartEditorInterface::typeSubParameter(SEntity *, void *prop, xsize i, QString& name, QWidget *&widget) const
  {
  if(i == SubName)
    {
    name = "Name";
    widget = new PropertyNameEditor((SProperty*)prop);
    }
  else if(i == SubType)
    {
    name = "Type";
    widget = new QComboBox;
    }
  else
    {
    xAssertFail();
    }
  }
