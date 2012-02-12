#include "syimagenode.h"
#include "QImage"

S_IMPLEMENT_PROPERTY(SyImageNode)

SPropertyInformation *SyImageNode::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageNode>("SyImageNode");

  SyImageOutput::InstanceInformation *outputInst = info->child(&SyImageNode::output);
  outputInst->setCompute(computeImage);

  FilenameProperty::InstanceInformation *filenameInst = info->add(&SyImageNode::filename, "filename");
  filenameInst->setMode(SPropertyInstanceInformation::UserSettable);
  filenameInst->setAffects(outputInst);

  BoolProperty::InstanceInformation *premultiplyInst = info->add(&SyImageNode::premultiply, "premultiply");
  premultiplyInst->setMode(SPropertyInstanceInformation::UserSettable);
  premultiplyInst->setAffects(outputInst);

  return info;
  }

SyImageNode::SyImageNode()
  {
  }

void SyImageNode::computeImage(const SPropertyInstanceInformation *, SyImageNode* im)
  {
  im->output.loadQImage(QImage(im->filename()), im->premultiply());
  }
