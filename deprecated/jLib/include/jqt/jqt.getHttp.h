#ifndef JLIB_JQT_GETHTTP_H
#define JLIB_JQT_GETHTTP_H

#include "jqt/jqt.types.h"

#ifdef QT_NETWORK_LIB

#include <QtCore/QObject>
#include <QtNetwork/QHttp>
// No macros as moc cant handle them
namespace jLib
{
namespace jqt
{

class getHttp : public QObject
    {
    Q_OBJECT
public:
    getHttp( string domain, string file="index.html", int port=80, bool ssl=0,
        string username="", string password="" );
    ~getHttp( );
    QHttp *http;
public slots:
    void httpRequestFinished( int, bool );
signals:
    void requestFinished( string );
private:
    int requestId;
    int domainRequestId;
    };

}
}

#endif
#endif

