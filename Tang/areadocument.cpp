#include "areadocument.h"
#include "spropertyinformationhelpers.h"
#include "area.h"
#include "tangmanager.h"

S_IMPLEMENT_PROPERTY(AreaDocument, Tang)

void AreaDocument::createTypeInformation(SPropertyInformationTyped<AreaDocument> *,
                                         const SPropertyInformationCreateData &)
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
