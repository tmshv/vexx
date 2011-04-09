#ifndef SYIMAGENODE_H
#define SYIMAGENODE_H

#include "synode.h"
#include "sbaseproperties.h"
#include "syimageoutput.h"

class SyImageNode : public SyNode
  {
  S_ENTITY(SyImageNode, SyNode, 0);
public:
  SyImageNode();

  StringProperty filename;
  SyImageOutput output;

private:
  static void computeImage( const SPropertyInstanceInformation *, SPropertyContainer * );
  };

#endif // SYIMAGENODE_H
