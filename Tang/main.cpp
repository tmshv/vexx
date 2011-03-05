#include "acore.h"
#include "aplugin.h"
#include "UIPlugin.h"
#include "Viewport.h"

int main( int argc, char **argv )
  {
  ACore app( argc, argv );
  app.addDirectory(ACore::rootPath());
  app.load("UI");

  APlugin<UIPlugin> ui(app);
  if(ui.isValid())
    {
    ui->addSurface(new Viewport);
    }

  return app.execute();
  }
