#include "acore.h"
#include "aplugin.h"
#include "UIPlugin.h"
#include "Viewport.h"
#include "assettree.h"
#include "application.h"

int main( int argc, char **argv )
  {
  ACore app( argc, argv );
  app.addDirectory(ACore::rootPath());
  app.load("UI");
  app.load("script");

  Application envApp;

  APlugin<UIPlugin> ui(app, "ui");
  if(ui.isValid())
    {
    ui->addSurface(new Viewport(&envApp));
    ui->addSurface(new AssetTree(&envApp));
    }

  return app.execute();
  }
