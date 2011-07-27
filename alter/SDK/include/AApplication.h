#ifndef AAPPLICATION_H
#define AAPPLICATION_H

#include "QApplication"
#include "AGlobal.h"

class APrivateWindow;
class APlugin;
class QSplashScreen;

class ALTER_EXPORT AApplication : public QApplication
    {
public:
    enum PluginMode { ForcedPlugins, UserPlugins };
    enum CreateMenuMode { AutomaticCreateMenu, NoCreateMenu };
    AApplication( QString applicationName, QString icon, QString splashScreen, int &argc, char **argv, PluginMode, CreateMenuMode );

    int exec( QString=QString(), QByteArray=QByteArray() );
    APlugin *sdk();

private:
    PluginMode _pluginMode;
    APrivateWindow *_window;
    QSplashScreen *_splash;
    };

#endif // AAPPLICATION_H
