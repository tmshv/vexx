#include "acore.h"
#include "aplugin.h"
#include "ScPlugin.h"
#include "splugin.h"
#include "syimagenode.h"
#include "syviewernode.h"
#include "UIPlugin.h"
#include "sypreviewviewer.h"

int main(int argc, char *argv[])
  {
  ACore app( argc, argv );

  app.addDirectory(ACore::rootPath());

  app.load("script");

  // this will work in debug only...
  app.addDirectory(app.rootPath() + "/../Synapse/scripts/");

  APlugin<SPlugin> shift(app, "db");
  if(shift.isValid())
  {
    shift->db().addType<SyImageNode>();
    shift->db().addType<SyViewerNode>();

    APlugin<UIPlugin> ui(app, "ui");
    if(ui.isValid())
    {
      ui->addSurface(new SyPreviewViewer(&shift->db()));
    }
  }

  APlugin<ScPlugin> script(app, "script");
  if(script.isValid())
  {
    // more like this in release...
    // script->includeFolder(app.rootPath() + "/scripts/");

    script->includeFolder(app.rootPath() + "/../Synapse/scripts/");
  }


  return app.execute();
  }
