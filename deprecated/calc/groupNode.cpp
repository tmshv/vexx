#include "groupNode.h"

groupNode::groupNode()
    {
    INIT_NODE

    // add call pointer property
    addProperty( "callPointer", "", propertyInfo::String, "Call Pointer" );
    property("callPointer").hide();

    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(updateInternalProperties(APropertyDevice*,AProperty*)) );

    _loading = false;

    // setup help
    property( "longHelp" ) = "A Group nodes allows you to place other nodes inside it, creating in effect a new node.\n\n Group nodes can be saved to the toolbar and used like other nodes.";
    property( "shortHelp" ) = "A Group nodes allows you to place other nodes inside it, creating in effect a new node.\n\n Group nodes can be saved to the toolbar and used like other nodes.";
    }

void groupNode::relocated()
    {
    nodeEntity::relocated();

    _loading = true;
    // search properties
    foreach( AProperty *prop, properties() )
        {
        if( prop->attributes().contains("forwardAttributes") )
            {
            XVariantList list( prop->attribute("forwardAttributes")->toList() );

            // try to set up each property from internal ones
            foreach( AVariant var, list )
                {
                XVariantList varTemp( var.toList() );
                AEntitySearch search( array()->search().findEntity( location().path() + "/" + varTemp[0].toString() ) );

                if( search.size() && search[0].propertyExists( varTemp[1].toString() ) )
                    {
                    // set the external property from the internal one found
                    *prop = search[0].property( varTemp[1].toString() ).value();
                    }
                }
            }
        }
    _loading = false;
    }

void groupNode::updateInternalProperties( APropertyDevice *, AProperty *prop )
    {
    // set the internal properties connected to prop
    if( _loading == false && prop->attributes().contains("forwardAttributes") )
        {
        XVariantList list( prop->attribute("forwardAttributes")->toList() );

        // for each connected node
        foreach( AVariant var, list )
            {
            XVariantList varTemp( var.toList() );
            AEntitySearch search( array()->search().findEntity( location().path() + "/" + varTemp[0].toString() ) );

            // if it exists, set it
            if( search.size() && search[0].propertyExists( varTemp[1].toString() ) )
                {
                search[0].property( varTemp[1].toString() ) = prop->value();
                }
            }
        }
    }

void groupNode::onChildrenChange()
    {
    // search for the call pointer node, and reset us if it cant be found
    if( property( "callPointer" )->toString() != "" )
        {
        AEntitySearch search( array()->search().findEntity( AEntityLocation( location(), property( "callPointer" )->toString() ) ) );
        if( search.size() == 0 )
            {
            property( "callPointer" ) = "";
            }
        }
    }
