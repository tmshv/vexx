#ifndef SYIMAGENODE_H
#define SYIMAGENODE_H

#include "sysourcenode.h"
#include "sbaseproperties.h"
#include "syimageoutput.h"

class SyImageNode : public SySourceNode
  {
  S_ENTITY(SyImageNode, SySourceNode, 0);

public:
  SyImageNode();

  BoolProperty premultiply;
  StringProperty filename;

private:
  static void computeImage( const SPropertyInstanceInformation *, SyImageNode * );
  };

#endif // SYIMAGENODE_H
