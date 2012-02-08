#include "sydocument.h"

S_IMPLEMENT_PROPERTY(SyDocument)

SPropertyInformation *SyDocument::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyDocument>("SyDocument");

  return info;
  }

SyDocument::SyDocument()
  {
  }

QWidget *SyDocument::createEditor()
  {
  return 0;
  }
