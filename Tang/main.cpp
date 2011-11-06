#include "acore.h"
#include "aplugin.h"
#include "scplugin.h"
#include "splugin.h"
#include "UIPlugin.h"
#include "Viewport.h"
#include "webview.h"
#include "GraphicsCore.h"
#include "saparteditor.h"

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


  SEntity *objectParent = &db->db().document;

  WebView *webData = new WebView(objectParent);
  script->registerScriptGlobal(webData);

  Viewport *vp = new Viewport(*db);
  ui->addSurface(vp);

  QObject::connect(webData, SIGNAL(objectChanged(Object *)), vp, SLOT(setObject(Object *)));

  ui->addSurface(webData);

  ui->show();

  SPartEditor* editor = SPartEditor::editNewPart("SEntity", "", &db->db().document);
  editor->show();

  SPartEditor* editor2 = SPartEditor::editNewPart("GCShader", "", &db->db().document);
  editor2->show();

  return app.execute();
  }
