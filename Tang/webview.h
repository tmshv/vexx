#ifndef WEBVIEW_H
#define WEBVIEW_H

#include "UISurface.h"
#include "sentityweakpointer.h"

class Object;
class QWebView;

class WebView : public QObject, public UISurface
  {
  Q_OBJECT

XProperties:
  XROProperty(Object *, currentObject);

public:
  WebView(SEntity *objectParent);

public slots:
  void loadObject(const QString& id);
  void load(const QString& url);

  void updateArea();

signals:
  void objectChanged(Object *object);

private slots:
  void loadFinished(bool ok);

private:
  QWebView *_webView;
  SEntityWeakPointer _objectParent;
  };

#endif // WEBVIEW_H
