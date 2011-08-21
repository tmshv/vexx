#include "acore.h"
#include "scplugin.h"
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

  APlugin<ScPlugin> script(app, "script");
  if(script.isValid())
    {
    script->include("startup.js");
    script->loadPlugin("ui");

    APlugin<UIPlugin> ui(app, "ui");
    if(ui.isValid())
      {
      ui->show();
      }
    }

  return app.execute();
  }
