#include "webview.h"
#include "QWebView"
#include "QNetworkAccessManager"
#include "QNetworkRequest"
#include "object.h"

#define PERMANENT_ADDRESS "http://goo.gl/DWBZJ"

#define baseHost() QString("ec2-46-137-71-148.eu-west-1.compute.amazonaws.com")
#define baseUrlWithHttp() QString("http://ec2-46-137-71-148.eu-west-1.compute.amazonaws.com/")

WebView::WebView(SEntity *objectParent) : UISurface("Explore", _webView = new QWebView, UISurface::Dock)
  {
  _objectParent = objectParent;

  setObjectName("tang");
  load(PERMANENT_ADDRESS);

  connect(_webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
  }

void WebView::load(const QString& url)
  {
  _webView->load(url);
  }

void WebView::loadObject(const QString &id)
  {
  QString url = baseUrlWithHttp() + "?current=" + id;
  _webView->load(url);
  }

void WebView::loadFinished(bool ok)
  {
  QUrl url = _webView->url();
  QString host = url.host();
  QString base(baseHost());
  if(host.right(base.length()) == base && url.hasQueryItem("current"))
    {
    QByteArray idData = QByteArray::fromHex(url.queryItemValue("current").toAscii());

    SEntity *objectParent = _objectParent.entity();
    if(idData.length() == 16 && objectParent)
      {
      Object *object = 0;

      // switch on object type?
      Area *area = objectParent->addChild<Area>("");
      object = area;

      if(object)
        {
        object->setName(idData.toHex());
        _currentObject = object;

        emit objectChanged(object);
        }
      }

    }
  }

void WebView::updateArea()
  {
#define BOUNDARY "AaB03x"
  QByteArray data = "baadF00d";
  QByteArray objectId = "QANG CHUNG?";

  QNetworkRequest request(baseUrlWithHttp());
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
