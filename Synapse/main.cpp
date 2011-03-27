#include "acore.h"
#include "aplugin.h"
#include "ScPlugin.h"
#include "QDebug"

int main(int argc, char *argv[])
  {
  ACore app( argc, argv );

  app.addDirectory(ACore::rootPath());

  app.load("script");

  APlugin<ScPlugin> script(app, "script");

  // this will work in debug only...
  app.addDirectory(app.rootPath() + "/../Synapse/scripts/");

  // more like this in release...
  // script->includeFolder(app.rootPath() + "/scripts/");

  script->includeFolder(app.rootPath() + "/../Synapse/scripts/");


  return app.execute();
  }
