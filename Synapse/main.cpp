#include "acore.h"
#include "aplugin.h"
#include "ScPlugin.h"
#include "splugin.h"
#include "syimagenode.h"
#include "syimagetexture.h"
#include "UIPlugin.h"
#include "sypreviewviewport.h"
#include "sydocument.h"
#include "syinterface.h"

int main(int argc, char *argv[])
  {
  ACore app("vexx.googlecode.com", "Synapse2.0", argc, argv);

  app.addDirectory(ACore::rootPath());

  app.load("script");
  app.load("synapsecore");

  APlugin<SPlugin> shift(app, "db");
  if(shift.isValid())
  {
    STypeRegistry::addType(SySourceNode::staticTypeInformation());
    STypeRegistry::addType(SyImageNode::staticTypeInformation());
    STypeRegistry::addType(SyPreviewViewport::staticTypeInformation());
    STypeRegistry::addType(SyImageTexture::staticTypeInformation());
    STypeRegistry::addType(SyDocument::staticTypeInformation());
  }

  SyInterface synapseInterface;
  APlugin<ScPlugin> script(app, "script");
  if(script.isValid())
  {
    // more like this in release...
    // script->includeFolder(app.rootPath() + "/scripts/");

    script->registerScriptGlobal("synapse", &synapseInterface);

    script->includeFolder(app.rootPath() + "/../Synapse/scripts/");
  }

  // this will work in debug only...
  app.addDirectory(app.rootPath() + "/../Synapse/scripts/");


  return app.execute();
  }
