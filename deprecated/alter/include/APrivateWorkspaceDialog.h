#ifndef WORKSPACEDIALOG_H
#define WORKSPACEDIALOG_H

#include <QDialog>

/** \cond INTERNAL
  */

namespace Ui {
    class workspaceDialog;
}

class APrivateWindow;
class QMenu;

class APrivateWorkspaceDialog : public QDialog
    {
    Q_OBJECT
public:
    APrivateWorkspaceDialog( APrivateWindow *parent = 0 );
    ~APrivateWorkspaceDialog();

    void initiate( QByteArray );
    void uninitiate();

    QMenu *menu();

public slots:
    void updateWorkspaceMenu();
    void setWorkspace( QString );


protected:
    void showEvent( QShowEvent * );

private slots:
    void updateList();
    void menuTriggered( QAction * );
    void removeCurrentItem();
    void saveAsCurrent();
    void saveCurrent();
    void applyCurrent();

private:
    Ui::workspaceDialog *ui;
    APrivateWindow *_parent;
    QMenu *_menu;
    };

/** \endcond
  */

#endif // WORKSPACEDIALOG_H
