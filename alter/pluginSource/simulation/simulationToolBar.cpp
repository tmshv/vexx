#include "simulationToolBar.h"
#include "simulationManager.h"
#include "ALog"
#include "abstractSimulator.h"
#include "timeDock.h"
#include "frameLogDock.h"
#include "viewportDock.h"

#include "QAction"
#include "QThread"
#include "QHBoxLayout"
#include "QToolButton"
#include "QInputDialog"
#include "QProgressBar"
#include "QCheckBox"
#include "QTimer"

#include "QDebug"

class simThread : public QThread
    {
    jProperty( simulationToolBar *, dock )
    simThread( simulationToolBar *d ) : QThread( d ), _dock( d ) {}
    void run()
        {
        _dock->simulate();
        }
    };

simulationToolBar::simulationToolBar() : _currentSimThread( 0 )
    {
    _manager = new simulationManager( );
    connect( _manager, SIGNAL(log(QString)), this, SLOT(log(QString)) );
    connect( _manager, SIGNAL(reset()), this, SLOT(reset()) );

    _simulate = addAction( QIcon(":/simulate.png"), "Simulate", this, SLOT(simulateClicked()) );
    addAction( QIcon(":/reset.png"), "Reset Simulation", this, SLOT(reset()) );

    _dragTime = addAction( QIcon( ":/dragSimulation.png" ), "Drag time with simulation" );
    _dragTime->setCheckable( TRUE );
    _dragTime->setChecked( FALSE );

    connect( this, SIGNAL(finished()), this, SLOT(simulationComplete()) );
    connect( this, SIGNAL(stepped(bool)), this, SLOT(updateTime()) );
    connect( this, SIGNAL(stepped(bool)), this, SLOT(updateSimulationView(bool)) );
    connect( this, SIGNAL(simulationLog(QString)), this, SLOT(log(QString)) );
    }

simulationToolBar::~simulationToolBar()
    {
    delete _manager;
    }

void simulationToolBar::added()
    {
    _manager->setApp( app() );
    connect( app()->accessAs<timeDock *>("time"), SIGNAL(timeChanged(APropertyDevice*,AProperty*)), this, SLOT(timeChanged(APropertyDevice*,AProperty*)) );
    }

void simulationToolBar::simulateClicked()
    {
    if( !_currentSimThread )
        {
        _simulate->setText( "Stop Simulating" );
        _simulate->setIcon( QIcon(":/stopSimulation.png") );
        _simulationTarget = app()->accessAs<timeDock *>("time")->entity()->property( "time/timeEnd" )->toDouble();

        if( _manager->mode() == abstractSimulator::Setup )
            {
            JFLOAT step( 1.0 / app()->accessAs<timeDock *>("time")->entity()->property( "time/fps" )->toDouble() );
            emit simulationLog( "Begin Simulation Setup" );
            FOREACHCONST( _manager->simulators(), sim )
                {
                (*sim)->setupSimulate();
                (*sim)->setStepTime( step );
                }
            _manager->mode( abstractSimulator::Simulation );
            emit simulationLog( "Setup Finished" );
            }

        _currentSimThread = new simThread( this );
        connect( _currentSimThread, SIGNAL(finished()), _currentSimThread, SLOT(deleteLater()) );
        _currentSimThread->start( );
        }
    else
        {
        stop();
        }
    }

void simulationToolBar::stop()
    {
    if( _currentSimThread )
        {
        _stop = 1;
        QTimer *timer( new QTimer( this ) );
        connect( timer, SIGNAL(timeout()), this, SLOT(stop()) );
        connect( timer, SIGNAL(timeout()), timer, SLOT(deleteLater()) );
        timer->start( 50 );
        return;
        }
    }

void simulationToolBar::reset()
    {
    if( _currentSimThread )
        {
        _stop = 1;
        QTimer *timer( new QTimer( this ) );
        connect( timer, SIGNAL(timeout()), this, SLOT(reset()) );
        connect( timer, SIGNAL(timeout()), timer, SLOT(deleteLater()) );
        timer->start( 50 );
        return;
        }
    if( _manager->mode() == abstractSimulator::Simulation && !_currentSimThread )
        {
        stopSimulating();
        FOREACHCONST( _manager->simulators(), sim )
            {
            (*sim)->setDisplayMode( abstractSimulator::Setup );
            (*sim)->clearSimulationCache( );
            }
        app()->accessAs<viewportDock *>("basicViewport")->refresh();
        _manager->mode( abstractSimulator::Setup );
        app()->accessAs<timeDock *>("time")->entity()->property( "time/currentTime" ) = 0;
        updateTime();
        }
    }

void simulationToolBar::updateTime()
    {
    app()->accessAs<timeDock *>("time")->entity()->property( "time/clipEnd" ) = maximumAvailableTime();
    }

void simulationToolBar::updateSimulationView( bool first )
    {
    if( first )
        {
        FOREACHCONST( _manager->simulators(), sim )
            {
            (*sim)->setDisplayMode( abstractSimulator::Simulation );
            (*sim)->setDisplayTime( maximumAvailableTime() );
            }
        app()->accessAs<viewportDock *>("basicViewport")->refresh();
        }
    if( _dragTime->isChecked() )
        {
        app()->accessAs<timeDock *>("time")->entity()->property( "time/currentTime" ) = maximumAvailableTime();
        setDisplayTime( app()->accessAs<timeDock *>("time")->entity()->property( "time/currentTime" )->toDouble() );
        }
    }

JFLOAT simulationToolBar::maximumAvailableTime()
    {
    if( _manager->simulators().size() == 1 )
        {
        return _manager->simulators().front()->maximumAvailableTime();
        }
    else
        {
        JFLOAT maxTime = jLib::floatLimits::nan();
        FOREACHCONST( _manager->simulators(), sim )
            {
            JFLOAT tempMax( (*sim)->maximumAvailableTime() );
            if( tempMax < maxTime || maxTime != maxTime )
                {
                maxTime = tempMax;
                }
            }
        if( maxTime != maxTime )
            {
            maxTime = 0;
            }
        return maxTime;
        }
    }

void simulationToolBar::simulationComplete( )
    {
    _currentSimThread = 0;
    _stop = 0;
    _simulate->setText( "Simulate" );
    _simulate->setIcon( QIcon(":/simulate.png") );
    }

void simulationToolBar::setDisplayTime( JFLOAT now )
    {
    bool isLogging = _logDock->isVisible();
    QString logText;
    FOREACHCONST( _manager->simulators(), sim )
        {
        (*sim)->setDisplayMode( abstractSimulator::Simulation );
        (*sim)->setDisplayTime( now );
        if( isLogging )
            {
            logText += (*sim)->currentTimeLogText() + "\n";
            }
        }
    app()->accessAs<viewportDock *>("basicViewport")->refresh();
    if( isLogging )
        {
        _logDock->setText( logText );
        }
    }

void simulationToolBar::log( QString log )
    {
    ALog << log << endl;
    }

void simulationToolBar::simulate()
    {
    if( _manager->simulators().size() > 0 )
        {
        _stop = 0;
        bool first = TRUE;
        emit simulationLog( "Begin Simulation" );
        while( maximumAvailableTime() < _simulationTarget )
            {
            FOREACHCONST( _manager->simulators(), sim )
                {
                if( _manager->simulators().size() == 1 || (*sim)->maximumAvailableTime() < _simulationTarget )
                    {
                    (*sim)->simulateStep();
                    }
                }
            emit stepped( first );
            first = FALSE;

            if( _stop == 1 )
                {
                break;
                }
            }
        emit simulationLog( "Finished Simulation" );
        emit finished( );
        }
    else
        {
        emit simulationLog( "No Simulators found" );
        }
    }

void simulationToolBar::timeChanged(APropertyDevice*,AProperty*)
    {
    JFLOAT now( app()->accessAs<timeDock *>("time")->entity()->property( "time/currentTime" )->toDouble() );
    if( now > 0 && now < maximumAvailableTime() )
        {
        setDisplayTime( now );
        }
    }

void simulationToolBar::stopSimulating()
    {
    }
