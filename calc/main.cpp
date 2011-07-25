#include "viewerDock.h"
#include "nodedock.h"
#include "nodeToolBar.h"

#ifdef SYNAPSE_RENDER_DEBUG
#include "renderDebugger.h"
#endif

#include "groupNode.h"
#include "cacheNode.h"
#include "colourPickerWidget.h"
#include "abstractQImageNode.h"
#include "nodeDB.h"
#include "nodeEntity.h"

ALTER_SDK_INITIALISE( Q_DECL_EXPORT )
    {
    // add the nodeBase type for loading purposes
    sdk->addCreator( nodeEntity::identifier(), nodeEntity::create );
    // add the qimage base node for possible copy and paste type events
    sdk->addCreator( abstractQImageNode::identifier(), abstractQImageNode::create );

    sdk->addGuiControlType( calcPropertyInfo::Colour, colourPickerWidget::create );
    sdk->addGuiControlType( calcPropertyInfo::ColourNoAlpha, colourPickerWidget::create );

    //add all the c++ node types
    nodeDB::registerNode( sdk, groupNode::identifier(), groupNode::create, groupNode::colour() );
    nodeDB::registerNode( sdk, cacheNode::identifier(), cacheNode::create, cacheNode::colour() );

    // add the node toolbar ( probably temporary )
    sdk->addToolBar( "Node Creation", new nodeToolBar );

    viewerDock *vD( new viewerDock );

    // add the docks
    sdk->addDockedItem( "Viewer", vD );
    sdk->addDockedItem( "Nodes", new nodeDock );

    nodeDB::initiateLibraryNodes( sdk );

    sdk->addPreferencesAction( vD->showPreferences() );

#ifdef SYNAPSE_RENDER_DEBUG
    sdk->addDockedItem( "Render Debug", new renderDebugger );
#endif

    return APlugin::Success;
    }

ALTER_SDK_UNINITIALISE( Q_DECL_EXPORT )
    {
    // remove all the stuff we added...

    sdk->removeCreator( nodeEntity::identifier() );

    nodeDB::unRegisterNode( sdk, groupNode::identifier() );
    nodeDB::unRegisterNode( sdk, cacheNode::identifier() );

    sdk->removeDockedItem( "Node Creation" );

    sdk->removeDockedItem( "Viewer" );
    sdk->removeDockedItem( "Nodes" );

#ifdef SYNAPSE_RENDER_DEBUG
    sdk->removeDockedItem( "Render Debug" );
#endif
    }
