#include "nodeExplorerDock.h"

#include "nodeExplorerWidget.h"
#include "QVBoxLayout"

nodeExplorerDock::nodeExplorerDock()
    {
    _widget = new nodeExplorerWidget( base );
    _layout = new QVBoxLayout( base );

    _layout->addWidget( _widget );
    _layout->setContentsMargins( 0, 0, 0, 0 );
    }
