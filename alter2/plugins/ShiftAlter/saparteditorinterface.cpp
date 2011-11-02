#include "saparteditorinterface.h"
#include "sentity.h"
#include "QLineEdit"
#include "QComboBox"

SDefaultPartEditorInterface::SDefaultPartEditorInterface() : SPartEditorInterface(true)
  {
  }

xsize SDefaultPartEditorInterface::numberOfTypeParameters(SEntity *prop) const
  {
  return NumberOfTypeParameters;
  }

void SDefaultPartEditorInterface::typeParameter(SEntity *, xsize index, QString& name, QWidget *&widget) const
  {
  if(index == Name)
    {
    name = "Name";
    widget = new QLineEdit;
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
