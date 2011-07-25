#include "jqt/jqt.getHttp.h"

#ifdef QT_NETWORK_LIB

namespace jLib
{
namespace jqt
{

getHttp::getHttp( string domain, string file, int port, bool ssl, string username,
        string password )
    {
    jAssert( port >= 0 );
    jAssert( domain != "" );
    jAssert( file != "" );

    http = new QHttp;
    QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp;
    if( ssl )
        {
        mode = QHttp::ConnectionModeHttps;
        }
    domainRequestId = http->setHost(QString::fromStdString( domain ), mode, port);
    if ( username != "" )
        {
        http->setUser(QString::fromStdString(username), QString::fromStdString(password));
        }
    requestId = http->get(QString::fromStdString(file));
    connect(http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));

    }

getHttp::~getHttp( )
    {
    delete http;
    }

void getHttp::httpRequestFinished( int id, bool error )
    {
    if( id == requestId )
        {
        if( error == TRUE )
            {
            jDebug<<http->errorString().toStdString()<<endl;
            emit requestFinished( "" );
            }
        else
            {
            emit requestFinished( QString( http->readAll() ).toStdString() );
            }
        }
    }

}
}

#endif
