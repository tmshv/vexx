#include "acore.h"
#include "aplugin.h"
#include "scplugin.h"
#include "splugin.h"
#include "UIPlugin.h"
#include "Viewport.h"
#include "webview.h"
#include "GraphicsCore.h"
#include "MeshCore.h"
#include "component.h"
#include "componentdocument.h"
#include "saparteditor.h"
#include "area.h"
#include "areadocument.h"

int main( int argc, char **argv )
  {
  ACore app( "vexx.googlecode.com", "Tang", argc, argv );

  app.addDirectory(ACore::rootPath());
  app.load("script");
  app.load("UI");

#ifdef X_DEBUG
  app.addDirectory(ACore::rootPath() + "../Tang");
#endif

  APlugin<ScPlugin> script(app, "script");
  if(!script.isValid())
    {
    return EXIT_FAILURE;
    }

  APlugin<UIPlugin> ui(app, "ui");
  if(!ui.isValid())
    {
    return EXIT_FAILURE;
    }

  APlugin<SPlugin> db(app, "db");
  if(!db.isValid())
    {
    return EXIT_FAILURE;
    }

  initiateMeshCore();

  //SPropertyArray *objectParent = &db->db().children;

  //WebView *webData = new WebView(objectParent);
  //script->registerScriptGlobal(webData);

  Viewport *vp = new Viewport(*db);
  ui->addSurface(vp);

  //QObject::connect(webData, SIGNAL(objectChanged(Object *)), vp, SLOT(setObject(Object *)));

  //ui->addSurface(webData);


  STypeRegistry::addType(Component::staticTypeInformation());
  STypeRegistry::addType(ComponentDocument::staticTypeInformation());
  STypeRegistry::addType(Area::staticTypeInformation());
  STypeRegistry::addType(AreaDocument::staticTypeInformation());
  Component::staticTypeInformation()->addStaticInterface(new ComponentEditorInterface);

  script->include("startupTang.js");

  ui->show();

  return app.execute();
  }
