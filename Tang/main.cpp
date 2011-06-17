#include "acore.h"
#include "aplugin.h"
#include "scplugin.h"
#include "UIPlugin.h"
#include "Viewport.h"
#include "assettree.h"
#include "application.h"
#include "webview.h"

int main( int argc, char **argv )
  {
  ACore app( "vexx.googlecode.com", "Tang", argc, argv );

  app.addDirectory(ACore::rootPath());
  app.load("UI");
  app.load("script");

  Application envApp;

  WebView *webData = new WebView();

  APlugin<ScPlugin> script(app, "script");
  if(script.isValid())
    {
    script->registerScriptGlobal(webData);
    }

  APlugin<UIPlugin> ui(app, "ui");
  if(ui.isValid())
    {
    ui->addSurface(new Viewport(&envApp));
    ui->addSurface(webData);

    ui->show();
    }

  return app.execute();
  }
