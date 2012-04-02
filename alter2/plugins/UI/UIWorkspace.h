#ifndef UILAYOUT_H
#define UILAYOUT_H

#include "UIGlobal.h"
#include "sreferenceentity.h"

class UI_EXPORT UIWorkspace : public SReferenceEntity
  {
  S_ENTITY(UIWorkspace, SReferenceEntity, 0)
public:
  UIWorkspace();
  };

S_PROPERTY_INTERFACE(UIWorkspace)

#endif // UILAYOUT_H
