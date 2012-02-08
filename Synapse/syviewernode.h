#ifndef SYVIEWERNODE_H
#define SYVIEWERNODE_H

#include "sentity.h"
#include "syimageinput.h"
#include "3D/GCTexture.h"

class SyViewerNode : public SEntity
  {
  S_ENTITY(SyViewerNode, SEntity, 0);
public:
  SyViewerNode();

  SyImageInput input;
  GCTexture preview;

private:
  static void computePreview(const SPropertyInstanceInformation *info, SPropertyContainer *cont);
  };

#endif // SYVIEWERNODE_H
