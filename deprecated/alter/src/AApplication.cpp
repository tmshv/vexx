#include "../SDK/AApplication"
#include "QSplashScreen"
#include "APrivateWindow.h"
#include "QPainter"
#include "QIcon"
#include "QTimeLine"
#include "QTimer"
#include "QApplication"
#include "QDesktopWidget"
#include "QImage"

/** \class AApplication
  \brief AApplication allows the programmer to start and manage the application. The programmer can disable dynamic loading of plugins, and force the loading of certain plugins to form an exact application.
  */

/** Create an appliation with name \p applicationName, \p splashScreen is used as a splash logo while the application loads.
  \p icon is an icon used for the window.
  */
AApplication::AApplication( QString applicationName, QString icon, QString splashScreen, int &argc, char **argv, PluginMode mode, CreateMenuMode menuMode )
        : QApplication( argc, argv ), _pluginMode( mode )
    {
    _splash = new QSplashScreen( QPixmap(splashScreen), Qt::WindowStaysOnTopHint );
    _splash->show();
    processEvents();

    _window = new APrivateWindow( QIcon(icon), applicationName, (_pluginMode == UserPlugins) ? true : false, (menuMode == AutomaticCreateMenu) ? true : false );
    }

int AApplication::exec( QString central, QByteArray recommendedSetup )
    {
    // initiate the workspace
    _window->restoreState( central, recommendedSetup );

    _window->show();
    _splash->finish(_window);

    int ret = QApplication::exec();
    delete _window;
    delete _splash;
    _window = 0;
    return ret;
    }

APlugin *AApplication::sdk()
    {
    xAssert( _window );
    return _window->sdk();
    }
