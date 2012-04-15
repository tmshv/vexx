#include "areadocument.h"
#include "area.h"

S_IMPLEMENT_PROPERTY(AreaDocument)

void AreaDocument::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  }

AreaDocument::AreaDocument()
  {
  }

QWidget *AreaDocument::createEditor()
  {
  return 0;
  }

void AreaDocument::newFile()
  {
  SDocument::newFile();

  addChild<Area>("Area");
  }
