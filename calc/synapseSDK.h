#ifndef SYNAPSESDK_H
#define SYNAPSESDK_H

#include "QtGlobal"
#include "APlugin"

#ifdef CALCV2_LIBRARY
#define SYNPASE_SDK_EXPORT Q_DECL_EXPORT
#else
#define SYNPASE_SDK_EXPORT Q_DECL_IMPORT
#endif

#include "nodeDB.h"

class SYNPASE_SDK_EXPORT synapseSDK
    {
public:
    synapseSDK( APlugin *sdk ) : _sdk( sdk ) {}
    template <typename T>void addNodeType()
        {
        nodeDB::registerNode( _sdk, T::identifier(), T::create, T::colour() );
        }
    template <typename T>void removeNodeType()
        {
        nodeDB::unRegisterNode( _sdk, T::identifier() );
        }

private:
    APlugin *_sdk;
    };

#define SYNAPSE_NODE_LIBRARY_INITIATE extern "C" void Q_DECL_EXPORT synapseInitFn( synapseSDK *sdk )
#define SYNAPSE_NODE_LIBRARY_UNINITIATE extern "C" void Q_DECL_EXPORT synapseUninitFn( synapseSDK *sdk )


#endif // SYNAPSESDK_H
