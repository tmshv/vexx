#include "switchNode.h"

inline QString getComponent( QString varA, QString varB, QCharRef in )
    {
    // we return a component value based on the character input
    switch( in.toAscii() )
        {
        case 'a': return varA;
        case 'b': return varB;
        case '0': return "0.0";
        case '1': return "1.0";
        };
    return "0.0";
    }

switchNode::switchNode()
    {
    INIT_NODE

    // add user inputs
    addInput("Input A", "Input A" );
    addInput("Input B", "Input B" );
    addProperty( "order", "aaaa", propertyInfo::String, "New Output" );

    QObject::connect( &property( "order" ), SIGNAL(onPropertyChange(AProperty*)), this, SLOT(updateSource()) );

    // default source
    property( "nodeEntity/source" ) = "return getInput(\"Input A\",coords);";

    property( "longHelp" ) = "A Switch allows the user to combine the channels from two nodes to create a new output. Accepted characters to \"New Output\" are a, b, 0, 1.";
    property( "shortHelp" ) = "A Switch allows the user to combine the channels from two nodes to create a new output. Accepted characters to \"New Output\" are a, b, 0, 1.";
    }

void switchNode::updateSource()
    {
    // set up source from properties
    QString input( "vec4 inpA;vec4 inpB;inpA = getInput(\"Input A\",coords);inpB = getInput(\"Input B\",coords);" );
    QString val( property("order")->toString().toLower() );

    // create a vec4 from the user input
    input += "return vec4( " + getComponent( "inpA.r", "inpB.r", val[0] ) + ", "
                      + getComponent( "inpA.g", "inpB.g", val[1] ) + ", "
                      + getComponent( "inpA.b", "inpB.b", val[2] ) + ", "
                      + getComponent( "inpA.a", "inpB.a", val[3] ) + " );";

    property( "nodeEntity/source" ) = input;
    }
