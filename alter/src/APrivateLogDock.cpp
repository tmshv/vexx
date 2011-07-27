#include "APrivateLogDock.h"
#include "APrivateWindow.h"
#include "../QtProject/ui_APrivateLogDock.h"
#include "QScrollBar"
#include "XMessageRouter"

/** \cond INTERNAL
  */

APrivateLogDock *g_logDock = 0;

/** \class APrivateLogDockHandler
  \brief This class is a reimplementation of an XMessageHandler that only handles messages of type APPLOG_ID and APPWARNING_ID
  */

/** \fn APrivateLogDockHandler::canHandle( int in )
  Returns true if the handler can handle messages of type \p in.
  */

/** \class APrivateLogDock
  \brief Handles messages for the log. Also prints warnings to the log.
  */

void printHandler(QtMsgType type, const char *msg)
  {
  if(g_logDock)
    {
    g_logDock->log(msg);
    }
  }

/** Create a APrivateLogDock
  */
APrivateLogDock::APrivateLogDock()
    {
    widget = new Ui::logDock( );
    widget->setupUi( base() );

    g_logDock = this;

    qInstallMsgHandler(printHandler);

    xRegisterMessageHandler( _handler );
    XMessageHandler::connect( &_handler, SIGNAL(handle(XString)), this, SLOT(log(XString)) );
    }

void APrivateLogDock::added()
    {
    }

/** Called when a log message arrives
  */
void APrivateLogDock::log( XString message )
    {
    widget->log->appendHtml( message );

    widget->log->verticalScrollBar()->setValue( widget->log->verticalScrollBar()->maximum() );

    app()->statusMessage( message, 1000 );
    }

/** \endcond
  */
