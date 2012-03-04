#include "synode.h"

S_IMPLEMENT_PROPERTY(SyNode)

SPropertyInformation *SyNode::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SyNode>("SyNode");

  return info;
  }

SyNode::SyNode()
{
}
