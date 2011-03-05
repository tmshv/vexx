#include "frameLogDock.h"

#include "QPlainTextEdit"
#include "QVBoxLayout"

frameLogDock::frameLogDock()
    {
    _layout = new QVBoxLayout( base() );
    _layout->setContentsMargins( 0, 0, 0, 0 );
    base()->setLayout( _layout );
    _text = new QPlainTextEdit( base() );
    _layout->addWidget( _text );
    }

void frameLogDock::setText( QString in )
    {
    _text->setPlainText( in );
    }
