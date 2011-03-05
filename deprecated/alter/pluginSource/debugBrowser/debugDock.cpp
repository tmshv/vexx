#include "debugDock.h"
#include "ui_debugDock.h"
#include "APlugin"
#include "entityModel.h"

debugDock::debugDock( ) : ui( new Ui::debugDock )
    {
    ui->setupUi( base() );
    }

void debugDock::added()
    {
    _model = new entityModel( app(), this );
    ui->treeView->setModel( _model );

    connect( ui->refresh, SIGNAL(clicked()), _model, SLOT(refresh()) );
    _model->refresh();
    }

debugDock::~debugDock()
    {
    delete ui;
    }
