#include "synapseSDK.h"

#include "imageNode.h"
#include "reorderNode.h"
#include "rotoshapeNode.h"
#include "switchNode.h"
#include "transformNodes.h"
#include "videoNode.h"

SYNAPSE_NODE_LIBRARY_INITIATE
    {
    // add all the nodes
    sdk->addNodeType<imageNode>( );
    sdk->addNodeType<reorderNode>( );
    sdk->addNodeType<rotoshapeNode>( );
    sdk->addNodeType<switchNode>( );
    sdk->addNodeType<scaleNode>( );
    sdk->addNodeType<rotateNode>( );
    sdk->addNodeType<translateNode>( );
    sdk->addNodeType<videoNode>( );
    }

SYNAPSE_NODE_LIBRARY_UNINITIATE
    {
    // remove all the nodes we added previously
    sdk->removeNodeType<imageNode>( );
    sdk->removeNodeType<reorderNode>( );
    sdk->removeNodeType<rotoshapeNode>( );
    sdk->removeNodeType<switchNode>( );
    sdk->removeNodeType<scaleNode>( );
    sdk->removeNodeType<rotateNode>( );
    sdk->removeNodeType<translateNode>( );
    sdk->removeNodeType<videoNode>( );
    }
