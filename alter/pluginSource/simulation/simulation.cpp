#include "simulation.h"
#include "simulationToolBar.h"
#include "frameLogDock.h"

#include "ALog"

ALTER_SDK_INITIALISE( SIMULATION_EXPORT )
    {
    if( sdk->request("time") )
        {
        simulationToolBar *sim = new simulationToolBar();
        frameLogDock *log = new frameLogDock();
        sim->logDock( log );
        sdk->addDockedItem( "Simulation Frame Log", log );
        sdk->addToolBar( "Simulation", sim );
        sdk->addCore( sim->manager() );
        return APlugin::Success;
        }
    return APlugin::Failure;
    }

ALTER_SDK_UNINITIALISE( SIMULATION_EXPORT )
    {
    sdk->removeDockedItem( "Simulation Frame Log" );
    sdk->removeToolBar( "Simulation" );
    }
