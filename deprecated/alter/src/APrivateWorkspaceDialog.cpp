#include "APrivateWorkspaceDialog.h"
#include "ui_APrivateWorkspaceDialog.h"
#include "APrivateWindow.h"
#include "QMenu"
#include "QSettings"
#include "QInputDialog"
#include "../SDK/ALog"

/** \cond INTERNAL
  */

/** \class APrivateWorkspaceDialog
  \brief Creates and manages the workspaces for the application
  */

/** Create the APrivateWorkspaceDialog with an appBase parent
  */
APrivateWorkspaceDialog::APrivateWorkspaceDialog( APrivateWindow *parent ) : QDialog(parent), ui(new Ui::workspaceDialog), _parent( parent ),
        _menu( new QMenu( "Workspaces", this ) )
    {
    ui->setupUi(this);
    // connect the menu to the menuTriggered slot
    connect( _menu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)) );

    // connect the menu options to the slots
    connect( ui->apply, SIGNAL(clicked()), this, SLOT(applyCurrent()) );
    connect( ui->saveAs, SIGNAL(clicked()), this, SLOT(saveAsCurrent()) );
    connect( ui->save, SIGNAL(clicked()), this, SLOT(saveCurrent()) );
    connect( ui->remove, SIGNAL(clicked()), this, SLOT(removeCurrentItem()) );
    }

/** Destory the workspaceDialog
  */
APrivateWorkspaceDialog::~APrivateWorkspaceDialog()
    {
    delete ui;
    }

/** Update the menu before display
  */
void APrivateWorkspaceDialog::updateWorkspaceMenu()
    {
    // empty the menu
    _menu->clear();

    // find the loadable workspaces
    QSettings settings;
    settings.beginGroup("Workspaces");
    QStringList keys = settings.childKeys();
    settings.endGroup();

    // add an action for each workspace
    foreach( QString k, keys )
        {
        _menu->addAction( k );
        }

    // if we added workspaces, add a separator
    if( keys.size() )
        {
        _menu->addSeparator();
        }

    // add a manager link
    _menu->addAction( tr("Workspace Manager"), this, SLOT( show() ) );
    }

/** Called before we show the dialog
  */
void APrivateWorkspaceDialog::showEvent( QShowEvent * )
    {
    updateList();
    }

/** Called to set the workspace to \p in.
  */
void APrivateWorkspaceDialog::setWorkspace( QString in )
    {

    QSettings settings;
    settings.beginGroup("Workspaces");

    // try to find the array
    QByteArray arr( settings.value( in ).toByteArray() );

    // if we found it
    if( !arr.isEmpty() )
        {
        // restore it!
        _parent->QMainWindow::restoreState( arr );
        }

    settings.endGroup();
    }

/** Called when amenu item is triggered
  */
void APrivateWorkspaceDialog::menuTriggered( QAction *in )
    {
    // if it isnt the workspace manager
    if( in->text() != tr("Workspace Manager") )
        {
        // set the workspace up
        setWorkspace( in->text() );
        }
    }

/** Remove the item highlighted in the UI
  */
void APrivateWorkspaceDialog::removeCurrentItem()
    {
    // find the selected items
    QList <QListWidgetItem *> items( ui->listWidget->selectedItems() );
    // if there is a selection
    if( items.size() )
        {
        // try to remove it from the settings
        QSettings settings;
        settings.beginGroup("Workspaces");
        settings.remove( items.front()->text() );
        settings.endGroup();
        }
    // update the menu
    updateWorkspaceMenu();
    // and the UI
    updateList();
    }

/** Save the current state to the currently selected item
  */
void APrivateWorkspaceDialog::saveCurrent()
    {
    // if there is a selected item
    QList <QListWidgetItem *> items( ui->listWidget->selectedItems() );
    if( items.size() )
        {
        // save it
        QSettings settings;
        settings.beginGroup("Workspaces");

        settings.setValue( items.front()->text(), _parent->saveState() );
        settings.endGroup();
        }
    else
        {
        // or save as
        saveAsCurrent();
        }
    }

/** Saves the current workspace setup as a new entry
  */
void APrivateWorkspaceDialog::saveAsCurrent()
    {
    QSettings settings;
    settings.beginGroup("Workspaces");
    QString text;
    // find a unique name
    do
        {
        bool ok;
        text = QInputDialog::getText( this, tr("Workspace Manager"), tr("Workspace Name"), QLineEdit::Normal,
                                                tr("Untitled Workspace"), &ok );
        if( !ok )
            {
            // user cancelled, so quit
            return;
            }
        } while( settings.contains( text ) );

    // set the value
    settings.setValue( text, _parent->saveState() );
    settings.endGroup();
    updateWorkspaceMenu();
    updateList();
    }

/** Apply the currently selected value as the current workspace
  */
void APrivateWorkspaceDialog::applyCurrent()
    {
    // find the selection
    QList <QListWidgetItem *> items( ui->listWidget->selectedItems() );
    if( items.size() )
        {
        // set the workspace
        setWorkspace( items.front()->text() );
        }
    }

/** Update the UI from settings
  */
void APrivateWorkspaceDialog::updateList()
    {
    // find the available options
    QSettings settings;
    settings.beginGroup("Workspaces");
    QStringList keys = settings.childKeys();
    settings.endGroup();

    // add each item
    ui->listWidget->clear();
    foreach( QString k, keys )
        {
        ui->listWidget->addItem( k );
        }
    }

/** Called on creation to initiate the menu
  */
void APrivateWorkspaceDialog::initiate( QByteArray recommended )
    {
    // update the menu
    updateWorkspaceMenu();

    // find the current workspace last close, and restore it
    QSettings settings;
    QByteArray arr( settings.value( "currentWorkspace" ).toByteArray() );

    if( !arr.isEmpty() )
        {
        _parent->QMainWindow::restoreState( arr );
        }
    else if( !recommended.isEmpty() )
        {
        _parent->QMainWindow::restoreState( recommended );
        }
    }

/** Called before destruction
  */
void APrivateWorkspaceDialog::uninitiate()
    {
    // save curent setup for restore next time
    QByteArray arr( _parent->saveState( ) );
    QSettings settings;
    settings.setValue( "currentWorkspace", arr );
    }

/** Return the menu this class manages and updates
  */
QMenu *APrivateWorkspaceDialog::menu()
    {
    return _menu;
    }

/** \endcond
  */
