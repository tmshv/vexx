#include "propertiesDock.h"
#include "AEntitySearch"
#include "QVBoxLayout"
#include "QMenu"

propertiesDock::propertiesDock( )
    {
    _layout = new QVBoxLayout( base() );
    _layout->setContentsMargins( 0, 0, 0, 0 );
    _mode = APropertyManager::Compact;
    _currentManager = 0;
    }

void propertiesDock::clear()
    {
    if( _currentManager )
        {
        delete _currentManager;
        }
    }

void propertiesDock::setMode( APropertyManager::propertyMode mode )
    {
    clear();
    _mode = mode;
    selectionChanged();
    }

void propertiesDock::added()
    {
    setMode( _mode );
    connect( app(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()) );
    }

void propertiesDock::selectionChanged()
    {
    base()->setUpdatesEnabled(false);

    clear();
    APropertyManager mgr;

    mgr.setMode( _mode );

    foreach( AEntity *ent, app()->selection() )
        {
        mgr.addEntity( ent );
        }

    _currentManager = mgr.createWidget();

    _currentManager->setParent( base() );
    _layout->addWidget( _currentManager );

    base()->setUpdatesEnabled(true);
    }
