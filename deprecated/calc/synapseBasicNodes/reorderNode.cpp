#include "reorderNode.h"

inline QString getComponent( QString var, QCharRef in )
    {
    //  return the component based on the character input
    switch( in.toAscii() )
        {
        case 'r': return var + ".r";
        case 'g': return var + ".g";
        case 'b': return var + ".b";
        case 'a': return var + ".a";
        case '0': return "0.0";
        case '1': return "1.0";
        };
    return "0.0";
    }

reorderNode::reorderNode()
    {
    INIT_NODE
    // add user inputs
    addInput("Input", "Input" );
    addProperty( "order", "rgba", propertyInfo::String, "New Order" );

    // connect for updates
    QObject::connect( &property( "order" ), SIGNAL(onPropertyChange(AProperty*)), this, SLOT(updateSource()) );

    // base source, until update source is called
    property( "nodeEntity/source" ) = "return getInput(\"Input\",coords);";

    property( "longHelp" ) = "A Reorder node allows the user to reorder the chanels of the input.\n\nEntering 4 character codes into the \"New Order\" property will change the output.\n\nSupported characters are r, g, b, a, 1, 0.";
    property( "shortHelp" ) = "A Reorder node allows the user to reorder the chanels of the input.\n\nEntering 4 character codes into the \"New Order\" property will change the output.\n\nSupported characters are r, g, b, a, 1, 0.";
    }

void reorderNode::updateSource()
    {
    // set up source
    QString input( "vec4 inp;inp = getInput(\"Input\",coords);" );
    QString val( property("order")->toString().toLower() );

    // source is a vec4 build from the user specified component values
    input += "return vec4( " + getComponent( "inp", val[0] ) + ", "
                      + getComponent( "inp", val[1] ) + ", "
                      + getComponent( "inp", val[2] ) + ", "
                      + getComponent( "inp", val[3] ) + " );";

    property( "nodeEntity/source" ) = input;
    }
