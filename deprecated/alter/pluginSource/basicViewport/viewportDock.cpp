#include "viewportDock.h"
#include "QVBoxLayout"
#include "QMenuBar"

#include "QDebug"

viewportDock::viewportDock()
    {
    }

viewportDock::~viewportDock()
    {
    }

void viewportDock::added()
    {
    _layout = new QVBoxLayout( base() );
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing( 0 );
    base()->setLayout( _layout );

    _menu = new QMenuBar( base() );
    _layout->setMenuBar( _menu );

    _ui = new viewport( base(), app() );
    connect( _ui, SIGNAL(contextActivate()), this, SIGNAL(contextActivate()) );
    _layout->addWidget( _ui );

    _showMenu = _menu->addMenu( "Show" );
    }

void viewportDock::addTool( viewportTool *in )
    {
    _ui->addTool( in );
    }

void viewportDock::removeTool( viewportTool *in )
    {
    _ui->removeTool( in );
    }

void viewportDock::addDrawType( QString in )
    {
    QAction *act( _showMenu->addAction( in ) );
    act->setCheckable( TRUE );
    act->setChecked( TRUE );
    connect( act, SIGNAL(triggered()), this, SIGNAL(showTypesChanged()) );
    connect( act, SIGNAL(triggered()), _ui, SLOT(update()) );
    _drawTypes.insert( in, act );
    }

void viewportDock::removeDrawType( QString in )
    {
    if( _drawTypes.contains( in ) )
        {
        delete _drawTypes.take( in );
        }
    }

bool viewportDock::shouldDraw( QString in )
    {
    if( _drawTypes.contains( in ) )
        {
        return _drawTypes[in]->isChecked();
        }
    return TRUE;
    }

void viewportDock::refresh()
    {
    _ui->update();
    _ui->refresh();
    }
