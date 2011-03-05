#include "syplugin.h"
#include "aplugin.h"
#include "splugin.h"
#include "sdatabase.h"
#include "GraphicsCore/GraphicsCore.h"

ALTER_PLUGIN(SynapseCorePlugin);

SynapseCorePlugin::SynapseCorePlugin()
  {
  }

void SynapseCorePlugin::load()
  {
  APlugin<SPlugin> shift(this, "db");
  if(shift.isValid())
    {
    SDatabase &db = shift->db();
    initiateGraphicsCore(&db);

    //db.addType<Test>();
    }
  }
