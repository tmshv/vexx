#ifndef SYSOURCENODE_H
#define SYSOURCENODE_H

#include "synode.h"
#include "syimageoutput.h"

class SYNAPSECORE_EXPORT SySourceNode : public SyNode
  {
  S_ENTITY(SySourceNode, SyNode, 0)

public:
  SySourceNode();

  SyImageOutput output;
  };

#endif // SYSOURCENODE_H
