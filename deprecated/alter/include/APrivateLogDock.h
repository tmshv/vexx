#ifndef LOGDOCK_H
#define LOGDOCK_H

#include "../SDK/AAbstractDock"
#include "../SDK/AGlobal"
#include "../SDK/ALog"
#include "XMessageHandler"

/** \cond INTERNAL
  */
namespace Ui
{
    class logDock;
}

class APrivateLogDockHandler : public XMessageHandler
    {
    virtual bool canHandle( int level ) const { return ( level == APPLOG_ID ); }
    };


class ALTER_EXPORT APrivateLogDock : public AAbstractDock
    {
    Q_OBJECT
public:
    enum logLevel { Information };
    APrivateLogDock( );

    virtual void added();

private slots:
    void log( XString );

private:
    Ui::logDock *widget;
    APrivateLogDockHandler _handler;
    };

/** \endcond
  */

#endif
