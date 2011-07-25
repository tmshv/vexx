#include "physicalalter.h"
#include "physicalAlterSimulation.h"
#include "simulationManager.h"
#include "objectEntity.h"
#include "groundObject.h"
#include "freezeBox.h"
#include "materialEntity.h"

#include "ALog"

static physicalAlterSimulation *sim = 0;

ALTER_SDK_INITIALISE( PHYSICALALTER_EXPORT )
    {
    // we rely on the simulation plugin
    if( sdk->request( "simulation" ) )
        {
        // add the Gui creators
        sdk->addCreator( freezeBox::identifier(), freezeBox::create );
        sdk->addCreator( objectEntity::identifier(), objectEntity::create );
        sdk->addCreator( groundObject::identifier(), groundObject::create );
        sdk->addCreator( materialEntity::identifier(), materialEntity::create );

        // add the simulator
        sim = new physicalAlterSimulation();
        sdk->accessAs<simulationManager *>( "simulation" )->addSimulator( sim );
        return APlugin::Success;
        }
    return APlugin::Failure;
    }

ALTER_SDK_UNINITIALISE( PHYSICALALTER_EXPORT )
    {
    // remove added classes
    if( sim )
        {
        sdk->removeCreator( freezeBox::identifier() );
        sdk->removeCreator( objectEntity::identifier() );
        sdk->removeCreator( groundObject::identifier() );
        sdk->removeCreator( materialEntity::identifier() );
        delete sim;
        }
    }
