#include "helpSystem.h"
#include "nodeEntity.h"

QString getLongHelp( nodeEntity *ent )
    {
    // put html into a nice format... probably expand on this function
    return "<html><head><title>Synapse " + ent->property( "nodeEntity/type" )->toString() + " Node Help</title></head><body><h1>" + ent->property( "nodeEntity/type" )->toString() + " Node</h1><p>" + ent->property( "longHelp" )->toString() + "</p></body></html>";
    }

QString getShortHelp( nodeEntity *ent )
    {
    // put html into a nice SHORT format... probably expand on this function
    return "<html><head><title>Synapse " + ent->property( "nodeEntity/type" )->toString() + " Node Help</title></head><body><h1>" + ent->property( "nodeEntity/type" )->toString() + " Node</h1><p>" + ent->property( "shortHelp" )->toString() + "</p></body></html>";
    }
