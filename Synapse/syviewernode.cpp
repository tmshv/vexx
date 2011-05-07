#include "syviewernode.h"

S_ENTITY_DEFINITION(SyViewerNode, SEntity)
  S_COMPUTE_GROUP(computeInputs)
    S_AFFECTS(preview)
  S_COMPUTE_GROUP_END()
  S_COMPUTABLE_PROPERTY_DEFINITION(SyImageInput, input, 0, computeInputs)
  S_COMPUTABLE_PROPERTY_DEFINITION(GCQImage, preview, computePreview, 0, QImage())
S_ENTITY_END_DEFINITION(SyViewerNode, SEntity)

SyViewerNode::SyViewerNode()
  {
  }

void SyViewerNode::computePreview(const SPropertyInstanceInformation *info, SPropertyContainer *cont)
  {
  SyViewerNode *viewer = cont->uncheckedCastTo<SyViewerNode>();
  viewer->preview = viewer->input.asQImage();
  }
