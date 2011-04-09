#include "syplugin.h"
#include "aplugin.h"
#include "splugin.h"
#include "sappdatabase.h"
#include "GraphicsCore/GraphicsCore.h"
#include "synode.h"
#include "syimagebase.h"
#include "syimageinput.h"
#include "syimageoutput.h"

ALTER_PLUGIN(SynapseCorePlugin);

SynapseCorePlugin::SynapseCorePlugin()
  {
  }

void SynapseCorePlugin::load()
  {
  APlugin<SPlugin> shift(this, "db");
  if(shift.isValid())
    {
    SAppDatabase &db = shift->db();
    initiateGraphicsCore(&db);

    db.addType<SyNode>();
    db.addType<SyImageBase>();
    db.addType<SyImageInput>();
    db.addType<SyImageOutput>();
    }
  }
