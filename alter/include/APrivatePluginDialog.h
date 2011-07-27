#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QtGui/QDialog>
#include <QLibrary>
#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>
#include <QFileInfo>
#include "../SDK/AGlobal"

/** \cond INTERNAL
  */

class APrivateWindow;
class QComboBox;

namespace Ui
{
    class pluginDialog;
}

class APlugin;

#define INIT_FN_NAME "alterInitialise"
#define UNINIT_FN_NAME "alterDeinitialise"

class ALTER_EXPORT APrivatePluginDialog : public QDialog
    {
    Q_OBJECT
public:
    typedef int (*initialiseFn)( APlugin * );
    typedef int (*deinitialiseFn)( APlugin * );
    enum pluginMode { DontLoad = 0, LoadOnDemand = 1, Load = 2, AlwaysLoad = 3 };
    APrivatePluginDialog( APrivateWindow *, bool );
    ~APrivatePluginDialog( );

    void addPluginDirectory( const QString & );

    bool request( QString );
    void *access( QString );
    void ensureLoaded( );

    void setCurrentCore( void * );

    QByteArray saveState();
    void restoreState( const QByteArray & );

protected:
    void showEvent( QShowEvent * );

protected slots:
    void refreshPluginModes();
    void refreshDialog();
    void clearPlugins( );

private:
    bool _userMode;
    bool tryLoad( QFileInfo, bool=FALSE );
    bool initPlugin( QLibrary * );
    void deinitPlugin( QLibrary * );
    void addSpacer( );
    void addDirectory( QString dirName );
    void addPlugin( QString pluginName, pluginMode );
    QStringList _directories;
    QHash <QString, QComboBox *> _currentList;
    struct loadedLibrary
        {
        QLibrary *lib;
        void *core;
        int level;
        };
    QHash <QString, loadedLibrary> _loadedLibrary;
    QScrollArea *_pluginsScroller;
    QWidget *_pluginsContent;
    QVBoxLayout *_pluginsContentLayout;
    Ui::pluginDialog *m_ui;
    APrivateWindow *_window;
    loadedLibrary *_currentLoading;
    };

/** \endcond
  */

#endif
