#include "saparteditorinterface.h"
#include "spropertycontainer.h"
#include "QLineEdit"
#include "QComboBox"

SDefaultPartEditorInterface::SDefaultPartEditorInterface() : SPartEditorInterface(true)
  {
  }

xsize SDefaultPartEditorInterface::numberOfTypeParameters(SProperty *prop) const
  {
  return NumberOfTypeParameters;
  }

void SDefaultPartEditorInterface::typeParameter(SProperty *, xsize index, QString& name, QWidget *&widget) const
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

void SDefaultPartEditorInterface::properties(SPropertyContainer *p, QStringList &l) const
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
