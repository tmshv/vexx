#include "simulationManager.h"
#include "abstractSimulator.h"
#include "ALog"

simulationManager::simulationManager( )
    {
    _mode = abstractSimulator::Setup;
    }

void simulationManager::addSimulator( abstractSimulator *in )
    {
    _simulators << in;

    connect( in, SIGNAL(destroyed(QObject*)), this, SLOT(removeSimulator(QObject*)) );
    connect( in, SIGNAL(logSignal(QString)), this, SIGNAL(log( QString )) );
    connect( in, SIGNAL(setupChangedSignal()), this, SIGNAL(reset( )) );

    in->app( _app );
    in->added();
    }

void simulationManager::setApp( APlugin *in )
    {
    _app = in;
    }

const QList <abstractSimulator *> &simulationManager::simulators()
    {
    return _simulators;
    }

void simulationManager::removeSimulator( abstractSimulator *in )
    {
    _simulators.removeAll( in );
    }

void simulationManager::removeSimulator( QObject *in )
    {
    _simulators.removeAll( static_cast <abstractSimulator *> (in) );
    }
