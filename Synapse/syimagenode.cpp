#include "syimagenode.h"
#include "QImage"

S_ENTITY_DEFINITION(SyImageNode, SyNode)
  S_COMPUTE_INPUTS(computeInputs)
    S_INPUT(filename)
  S_COMPUTE_INPUTS_END()
  S_COMPUTABLE_PROPERTY_DEFINITION(StringProperty, filename, 0, computeInputs, "")
  S_COMPUTABLE_PROPERTY_DEFINITION(SyImageOutput, output, computeImage, 0)
S_ENTITY_END_DEFINITION(SyImageNode, SyNode)

SyImageNode::SyImageNode()
  {
  }

void SyImageNode::computeImage(const SPropertyInstanceInformation *, SPropertyContainer* node)
  {
        SyImageNode* syImage = node->castTo<SyImageNode>();
        syImage->output.loadImage(QImage(syImage->filename.value()));
  }
