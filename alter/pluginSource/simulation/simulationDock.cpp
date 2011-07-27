#include "simulationToolBar.h"
#include "simulationManager.h"
#include "appLog.h"
#include "abstractSimulator.h"
#include "timeDock.h"

#include "QThread"
#include "QHBoxLayout"
#include "QToolButton"
#include "QInputDialog"
#include "QProgressBar"

class simThread : public QThread
    {
    jProperty( simulationToolBar *, dock )
    simThread( simulationToolBar *d ) : QThread( d ), _dock( d ) {}
    void run()
        {
        _dock->simulate();
        }
    };

simulationToolBar::simulationToolBar()
    {
    _manager = new simulationManager();

    QHBoxLayout *layout( new QHBoxLayout( base() ) );
    layout->setContentsMargins( 0, 0, 0, 0 );
    base()->setLayout( layout );

    QToolButton *simulate( new QToolButton( base() ) );
    layout->addWidget( simulate );
    simulate->setText( "Simulate" );
    QObject::connect( simulate, SIGNAL(clicked()), this, SLOT(simulateClicked()) );

    connect( this, SIGNAL(finished()), this, SLOT(simulationComplete()) );
    }

simulationToolBar::~simulationToolBar()
    {
    delete _manager;
    }

void simulationToolBar::simulateClicked()
    {
    JFLOAT now( app().accessAs<timeDock *>("time")->entity()->property( "time/currentTime" )->toDouble() );
    _simulationTarget = QInputDialog::getDouble( base(), "Simulate", "Simulate Until (seconds)", now, now, app().accessAs<timeDock *>("time")->entity()->property( "time/timeEnd" )->toDouble(), 2 );

    simThread *th( new simThread( this ) );
    connect( th, SIGNAL(finished()), th, SLOT(deleteLater()) );
    connect( this, SIGNAL(stepped()), this, SLOT(updateTime()) );
    th->start();
    }

void simulationToolBar::updateTime()
    {
    app().accessAs<timeDock *>("time")->entity()->property( "time/clipEnd" ) = maximumAvailableTime();
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
        return maxTime;
        }
    }

void simulationToolBar::simulationComplete( )
    {
    appLog << "Simulation Complete" << endl;
    }

void simulationToolBar::simulate()
    {
    JFLOAT step( 1.0 / app().accessAs<timeDock *>("time")->entity()->property( "time/fps" )->toDouble() );

    FOREACHCONST( _manager->simulators(), sim )
        {
        (*sim)->setStepTime( step );
        }

    while( maximumAvailableTime() < _simulationTarget )
        {
        FOREACHCONST( _manager->simulators(), sim )
            {
            if( _manager->simulators().size() == 1 || (*sim)->maximumAvailableTime() < _simulationTarget )
                {
                (*sim)->simulateStep();
                }
            }
        emit stepped( );
        }
    emit finished( );
    }
