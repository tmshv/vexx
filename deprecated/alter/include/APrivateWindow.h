#ifndef APPBASE_H
#define APPBASE_H

/** \cond INTERNAL
  */

#include "../SDK/AGlobal"
#include "QMainWindow"
#include "APrivateWindow.h"
#include "APrivatePluginDialog.h"
#include "APrivateWorkspaceDialog.h"
#include "APrivateDocument.h"
#include "XMessageHandler"

namespace Ui
{
    class appBase;
}

class APrivateWindowWarningHandler : public XMessageHandler
    {
    virtual bool canHandle( int level ) const { return ( level == APPWARNING_ID ); }
    };

class APrivateLogDock;
class APrivateCustomDockWidget;

class ALTER_EXPORT APrivateWindow : public QMainWindow
    {
    Q_OBJECT
public:
    APrivateWindow( QIcon, QString, bool userPluginInterface, bool needsCreateMenu, QString="jorjSoftware", QString="placid-acid.com", QString="1.0", QWidget *parent = 0);
    ~APrivateWindow();

    APrivatePluginDialog *pluginManager();

    void addDockedItem( QString, AAbstractDock * );
    void removeDockedItem( QString );

    void addToolBar( QString, AAbstractToolBar * );
    void removeToolBar( QString );

    APrivateDocument &document();
    const APrivateDocument &document() const;

    void statusMessage( QString, unsigned int=0 );

    QMenu *fileMenu();
    QMenu *windowMenu();
    QMenu *createMenu();

    void setContextDock( AAbstractDock * );

    void addRecentFile( QString );

    APlugin *sdk();

    void restoreState( QString, QByteArray );

    void addPreferencesAction( QAction *act );

protected:
    bool eventFilter(QObject *, QEvent *);
    virtual void closeEvent( QCloseEvent * );
    void setCentralDock( AAbstractDock * );
    void clearCentralDock( );

private slots:
    void handle( XString );
    void cut();
    void copy();
    void paste();
    void deleteSelection();
    void openRecent( QAction* );
    void modifiedChanged( bool );
    void rebuildWindowMenu( );
    void rebuildFileMenu( );
    void rebuildCreateMenu( );
    void pollWindowMenuStates( QAction * );
    void createTriggered( QAction * );
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void setCentralDock();

private:
    APrivateDocument _document;
    APrivatePluginDialog *_plugins;
    APrivateWorkspaceDialog *_workspace;
    bool discardChanges( );
    void setActiveFile( QString, QString );
    void refreshWindowTitle();
    QString _currentFilename;
    QString _currentExtension;
    Ui::appBase *ui;
    APrivateLogDock *_log;
    QString _backupLog;
    AAbstractDock *_centralDock;
    AAbstractDock *_currentContextDock;
    QHash <QString, APrivateCustomDockWidget *> _qDocks;
    QHash <QString, AAbstractDock *> _dockedWidgets;
    QHash <QString, AAbstractToolBar *> _toolBars;
    APlugin _sdk;
    APrivateWindowWarningHandler _messageHandler;
    };

/** \endcond
  */
#endif
