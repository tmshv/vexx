#include "webview.h"
#include "QWebView"
#include "QNetworkAccessManager"
#include "QNetworkRequest"
#include "objectid.h"

#define baseUrl() QString("http://ec2-46-51-138-106.eu-west-1.compute.amazonaws.com/")

WebView::WebView() : UISurface("Explore", _webView = new QWebView, UISurface::Dock)
  {
  setObjectName("tang");
  load(baseUrl());

  connect(_webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished()));
  }

void WebView::load(const QString& url)
  {
  _webView->load(url);
  }

void WebView::loadObject(const ObjectId &object)
  {
  QString url = baseUrl() + "?current=" + object.toHex();
  _webView->load(url);
  }

void WebView::loadFinished()
  {
  }

void WebView::updateArea()
  {
#define BOUNDARY "AaB03x"
  QByteArray data = "baadF00d";
  QByteArray objectId = "QANG CHUNG?";

  QNetworkRequest request(baseUrl());
  request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data, boundary=" BOUNDARY);

  QByteArray content;

  content.append("--" BOUNDARY "\r\n");
  content.append("Content-Disposition: form-data; name=\"setAreaData\"\r\n\r\n");
  content.append("1\r\n");

  content.append("--" BOUNDARY "\r\n");
  content.append("Content-Disposition: form-data; name=\"id\"\r\n\r\n");
  content.append(objectId + "\r\n");

  content.append("--" BOUNDARY "\r\n");
  content.append("Content-Disposition: form-data; name=\"data\"; filename=\"areaData.bin\"\r\n");
  content.append("Content-Transfer-Encoding: binary\r\n\r\n");
  content.append(data + "\r\n");

  content.append("--" BOUNDARY "--");

  _webView->load(request, QNetworkAccessManager::PostOperation, content);
#undef BOUNDARY
  }
