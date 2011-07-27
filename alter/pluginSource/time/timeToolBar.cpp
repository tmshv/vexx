#include "timeToolBar.h"
#include "timeDock.h"
#include "QIcon"
#include "QAction"

#include "ALog"

timeToolBar::timeToolBar( timeDock *d ) : _dock( d )
    {
    addAction( QIcon(":/skipBw.png"), "Skip Backwards", this, SLOT(skipBw()) );

    addAction( QIcon(":/stepBw.png"), "Step Backwards", _dock, SLOT(stepBw()) )->setShortcut( QKeySequence( Qt::Key_Left ) );

    _playPause = addAction( QIcon(":/play.png"), "Play" );
    _playPause->setShortcut( QKeySequence( Qt::Key_Space ) );
    _playPause->setData( FALSE );
    connect( _playPause, SIGNAL(triggered()), this, SLOT(playPause()) );

    addAction( QIcon(":/stepFw.png"), "Step Forwards", _dock, SLOT(stepFw()) )->setShortcut( QKeySequence( Qt::Key_Right ) );

    addAction( QIcon(":/skipFw.png"), "Skip Forwards", this, SLOT(skipFw()) );
    }

void timeToolBar::playPause( )
    {
    if( _playPause->data().toBool() )
        {
        _playPause->setData( FALSE );
        _dock->pause();
        _playPause->setText( "Play" );
        _playPause->setIcon( QIcon(":/play.png") );
        }
    else
        {
        _playPause->setData( TRUE );
        _dock->play();
        _playPause->setText( "Pause" );
        _playPause->setIcon( QIcon(":/pause.png") );
        }
    }

void timeToolBar::skipBw( )
    {
    _playPause->setData( FALSE );
    _playPause->setText( "Play" );
    _playPause->setIcon( QIcon(":/play.png") );
    _dock->skipBw();
    }

void timeToolBar::skipFw( )
    {
    _playPause->setData( FALSE );
    _playPause->setText( "Play" );
    _playPause->setIcon( QIcon(":/play.png") );
    _dock->skipFw();
    }
