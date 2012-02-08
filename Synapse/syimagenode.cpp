#include "syimagenode.h"
#include "QImage"

S_IMPLEMENT_PROPERTY(SyImageNode)

SPropertyInformation *SyImageNode::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyImageNode>("SyImageNode");

  SyImageOutput::InstanceInformation *outputInst = info->add(&SyImageNode::output, "output");
  outputInst->setCompute(computeImage);

  StringProperty::InstanceInformation *filenameInst = info->add(&SyImageNode::filename, "filename");
  filenameInst->setAffects(outputInst);

  return info;
  }

SyImageNode::SyImageNode()
  {
  }

void SyImageNode::computeImage(const SPropertyInstanceInformation *, SPropertyContainer* node)
  {
  SyImageNode* syImage = node->castTo<SyImageNode>();
  syImage->output.loadQImage(QImage(syImage->filename.value()));
  }
