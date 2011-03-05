#include "scriptDock.h"
#include "ui_scriptDock.h"
#include "appLog.h"
#include "QToolBar"
#include "QScriptEngine"
#include "highlighter.h"

scriptDock::scriptDock( ) : baseDock( ), ui(new Ui::scriptDock), _appScript( this )
    {
    ui->setupUi(base);
    _toolbar = new QToolBar( "Script Tools", base );
    QAction *run = _toolbar->addAction( QIcon(":/run.png"), "Run", this, SLOT(run()) );
    run->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_Enter ) );
    _toolbar->addAction( QIcon(":/clear.png"), "Clear", ui->textEdit, SLOT(clear()) );
    _toolbar->setIconSize( QSize( 16, 16 ) );
    _toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    ui->verticalLayout->insertWidget( 0, _toolbar );

    _highlighter = new highlighter( ui->textEdit );
    };

void scriptDock::added( )
    {
    _appScript.app( app() );
    _appScript.added( );
    }

appScript *scriptDock::core()
    {
    return &_appScript;
    }

scriptDock::~scriptDock()
    {
    delete ui;
    }

void scriptDock::run()
    {
    QString code = ui->textEdit->toPlainText();
    if( ui->textEdit->hasFocus() && code != "" )
        {
        _appScript.execute( code );
        }
    }
