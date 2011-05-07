#include "syimagenode.h"
#include "QImage"

S_ENTITY_DEFINITION(SyImageNode, SyNode)
  S_COMPUTE_GROUP(computeInputs)
    S_AFFECTS(output)
  S_COMPUTE_GROUP_END()
  S_COMPUTABLE_PROPERTY_DEFINITION(StringProperty, filename, 0, computeInputs, "")
  S_COMPUTABLE_PROPERTY_DEFINITION(SyImageOutput, output, computeImage, 0)
S_ENTITY_END_DEFINITION(SyImageNode, SyNode)

SyImageNode::SyImageNode()
  {
  }

void SyImageNode::computeImage(const SPropertyInstanceInformation *, SPropertyContainer* node)
  {
  SyImageNode* syImage = node->castTo<SyImageNode>();
  syImage->output.loadQImage(QImage(syImage->filename.value()));
  }
