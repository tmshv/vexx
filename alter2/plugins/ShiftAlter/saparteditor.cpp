#include "saparteditor.h"
#include "styperegistry.h"
#include "sentity.h"

SPartEditor::SPartEditor(const QString &type, SProperty *parent)
  {
  }


SPartEditor *SPartEditor::editNewPart(const QString &type, const QString &name, SEntity *parent)
  {
  const SPropertyInformation *info = STypeRegistry::findType(type);
  xAssert(info);

  if(info)
    {
    SProperty *p = parent->addChild(info, name);

    return new SPartEditor(type, p);
    }

  return 0;
  }
