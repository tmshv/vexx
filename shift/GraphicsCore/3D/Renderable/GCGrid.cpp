#include "GCGrid.h"

S_IMPLEMENT_PROPERTY(GCGrid)

SPropertyInformation *GCGrid::createTypeInformation()
  {
  return SPropertyInformation::create<GCGrid>("GCGrid");
  }

GCGrid::GCGrid()
  {
  }

void GCGrid::render()
  {
  }
