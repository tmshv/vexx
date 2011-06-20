#include "GraphicsCore.h"
#include "GCImage.h"
#include "GCQImage.h"
#include "GCGeometry.h"

void initiateGraphicsCore(SDatabase *db)
  {
  db->addType<GCImage>();
  db->addType<GCQImage>();
  db->addType<GCPolygonArray>();
  db->addType<GCGeometry>();
  }
