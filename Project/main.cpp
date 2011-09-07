#include "acore.h"
#include "scplugin.h"
#include "splugin.h"
#include "aplugin.h"
#include "UIPlugin.h"

int main( int argc, char **argv )
  {
  ACore app( "vexx.googlecode.com", "Project", argc, argv );

  app.addDirectory(ACore::rootPath());

#ifdef X_DEBUG
  app.addDirectory(ACore::rootPath() + "/../Project/");
#endif

  app.load("script");

  APlugin<SPlugin> shift(app, "db");
  if(shift.isValid())
    {
    shift->setSetting("la", "ls", 54);
    }

  APlugin<ScPlugin> script(app, "script");
  if(script.isValid())
    {
    ScPlugin *sc = script.plugin();
    sc->include("startup.js");
    sc->load();
    }

  app.load("ui");
  APlugin<UIPlugin> ui(app, "ui");
  if(ui.isValid())
    {
    APlugin<SPlugin> db(app, "db");
    if(db.isValid())
      {
      //initiateGraphicsCore(&db->db());
      }

    ui->show();
    }

  return app.execute();
  }
