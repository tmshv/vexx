#include "acore.h"
#include "aplugin.h"
#include "scplugin.h"
#include "splugin.h"
#include "UIPlugin.h"
#include "Viewport.h"
#include "assettree.h"
#include "application.h"
#include "webview.h"
#include "GraphicsCore.h"

int main( int argc, char **argv )
  {
  ACore app( "vexx.googlecode.com", "Tang", argc, argv );

  app.addDirectory(ACore::rootPath());
  app.load("UI");
  app.load("script");

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

  initiateGraphicsCore(&db->db());


  Application envApp;

  SEntity *objectParent = &db->db().document;

  WebView *webData = new WebView(objectParent);
  script->registerScriptGlobal(webData);

  Viewport *vp = new Viewport(&envApp, *db);
  ui->addSurface(vp);

  QObject::connect(webData, SIGNAL(objectChanged(Object *)), vp, SLOT(setObject(Object *)));

  ui->addSurface(webData);

  ui->show();

  return app.execute();
  }
