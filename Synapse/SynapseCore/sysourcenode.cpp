#include "sysourcenode.h"

S_IMPLEMENT_PROPERTY(SySourceNode)

SPropertyInformation *SySourceNode::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<SySourceNode>("SySourceNode");

  info->add(&SySourceNode::output, "output");

  return info;
  }

SySourceNode::SySourceNode()
  {
  }
