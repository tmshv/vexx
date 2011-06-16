#ifndef WEBVIEW_H
#define WEBVIEW_H

#include "UISurface.h"

class ObjectId;
class QWebView;

class WebView : public QObject, public UISurface
  {
  Q_OBJECT

public:
  WebView();

public slots:
  void loadObject(const ObjectId &object);
  void load(const QString& url);

  void updateArea();

signals:
  void objectChanged(const ObjectId &object);

private slots:
  void loadFinished();

private:
  QWebView *_webView;
  };

#endif // WEBVIEW_H
