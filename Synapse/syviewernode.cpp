#include "syviewernode.h"

S_IMPLEMENT_PROPERTY(SyViewerNode)

SPropertyInformation *SyViewerNode::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyViewerNode>("SyViewerNode");

  GCTexture::InstanceInformation *previewInst = info->add(&SyViewerNode::preview, "preview");
  previewInst->setCompute(computePreview);

  SyImageInput::InstanceInformation *inputInst = info->add(&SyViewerNode::input, "input");
  inputInst->setAffects(previewInst);

  return info;
  }

SyViewerNode::SyViewerNode()
  {
  }

void SyViewerNode::computePreview(const SPropertyInstanceInformation *info, SPropertyContainer *cont)
  {
  SyViewerNode *viewer = cont->uncheckedCastTo<SyViewerNode>();

  //viewer->preview viewer->input.asQImage();
  }
