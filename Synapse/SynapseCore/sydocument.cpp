#include "sydocument.h"

S_IMPLEMENT_PROPERTY(SyDocument)

SPropertyInformation *SyDocument::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyDocument>("SyDocument");

  SyImageInput::InstanceInformation *inputInst = info->add(&SyDocument::input, "input");
  inputInst->setMode(SPropertyInstanceInformation::Input);


  return info;
  }

SyDocument::SyDocument()
  {
  }

QWidget *SyDocument::createEditor()
  {
  return 0;
  }
