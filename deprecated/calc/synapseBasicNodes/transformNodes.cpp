#include "transformNodes.h"
#include "nodeEntityManipulator.h"

transformNode::transformNode()
    {
    INIT_NODE

    property("editable") = true;

    // we add the property transform used by child classes to actually transform
    addProperty("transform", QMatrix3x3(), propertyInfo::Matrix3x3, "transform" );
    property("transform").hide();
    // call update transform when the property is set
    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(updateTransform(APropertyDevice*,AProperty*)) );

    // this attribute is used in the glsl code and is the inverted transform matrix
    addAttribute("glslTransform", QMatrix3x3(), propertyInfo::Matrix3x3 );
    property("glslTransform").hide();

    // the input to transform
    addInput( "input", "Input" );

    // the source
    property("nodeEntity/source") = "// grab the requested position in the image\n"
                                    "vec2 pos = ( getAttribute(\"glslTransform\") * vec3( coords, 1.0 ) ).st;\n"
                                    "\n"
                                    "// find the edge pixel positions to fin dtheir contributions\n"
                                    "vec2 a = floor( pos );\n"
                                    "vec2 d = ceil( pos );\n"
                                    "vec2 b = vec2( d.x, a.y );\n"
                                    "vec2 c = vec2( a.x, d.y );\n"
                                    "\n"
                                    "// make pos relative to a, for use in interpolation\n"
                                    "pos -= a - vec2( 0.5, 0.5 );\n"
                                    "\n"
                                    "// return the interpolated value\n"
                                    "// find the interpolation between a and b, c and d, then between a&b and c&d.\n"
                                    "//y interpolation\n"
                                    "return ( 1 - pos.y ) *\n"
                                    "                (\n"
                                    "                        // x interpolation\n"
                                    "                        ( getInput(\"input\", a ) * ( 1 - pos.x ) )\n"
                                    "                        + ( getInput(\"input\", b ) *  pos.x )\n"
                                    "                )\n"
                                    "        + ( pos.y *\n"
                                    "                (\n"
                                    "                        // x interpolation\n"
                                    "                        ( getInput(\"input\", c ) * ( 1 - pos.x ) )\n"
                                    "                        + ( getInput(\"input\", d ) *  pos.x )\n"
                                    "                )\n"
                                    "        );\n";
    }

QRectF transformNode::rect()
    {
    // the rect is the original matrix transformed
    QMatrix3x3 mat( property( "transform" )->toMatrix3x3() );
    QTransform trans( mat(0,0), mat(0,1), mat(0,2), mat(1,0), mat(1,1), mat(1,2), mat(2,0), mat(2,1), mat(2,2) );
    return trans.mapRect( nodeEntity::rect() );
    }

void transformNode::updateTransform( APropertyDevice *, AProperty *prop )
    {
    // if the changed property was "transform"
    if( prop->name() == "transform" )
        {
        // update the glslTransform property with the inverse
        QMatrix3x3 mat( property( "transform" )->toMatrix3x3() );
        QTransform trans( mat(0,0), mat(0,1), mat(0,2), mat(1,0), mat(1,1), mat(1,2), mat(2,0), mat(2,1), mat(2,2) );

        trans = trans.inverted();
        mat(0,0) = trans.m11();
        mat(0,1) = trans.m12();
        mat(0,2) = trans.m13();
        mat(1,0) = trans.m21();
        mat(1,1) = trans.m22();
        mat(1,2) = trans.m23();
        mat(2,0) = trans.m31();
        mat(2,1) = trans.m32();
        mat(2,2) = trans.m33();
        property("glslTransform") = mat;
        }
    }

scaleNode::scaleNode()
    {
    INIT_NODE
    // add custom attributes for scale node
    addAttribute( "Scale", XVector2D( 1.0, 1.0 ), propertyInfo::Component2D, "Scale" );
    addAttribute( "Translate", XVector2D( 0.0, 0.0 ), propertyInfo::Component2D, "Translate" );

    // we want a callback when our properties change
    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(updateTransform(APropertyDevice*,AProperty*)) );

    // add the manipulator to scale the source
    addManipulator( Manipulator( 1, 1 ) );


    property( "longHelp" ) = "A Scale node allows the user to scale the image about an arbitrary point.";
    property( "shortHelp" ) = "A Scale node allows the user to scale the image about an arbitrary point.";
    }

void scaleNode::updateTransform( APropertyDevice *dev, AProperty *prop )
    {
    transformNode::updateTransform( dev, prop );
    if( prop->name() == "Scale" || prop->name() == "Translate" )
        {
        // set up the transform matrix
        QMatrix3x3 mat, preMat, postMat;
        mat(0,0) = property( "Scale" )->toVector2D().x();
        mat(1,1) = property( "Scale" )->toVector2D().y();
        preMat(2,0) = property( "Translate" )->toVector2D().x();
        preMat(2,1) = property( "Translate" )->toVector2D().y();
        postMat(2,0) = -property( "Translate" )->toVector2D().x();
        postMat(2,1) = -property( "Translate" )->toVector2D().y();

        property("transform") = postMat * mat * preMat;
        }
    }

void scaleNode::manipulatorChanged( int )
    {
    // when a manipulator is changed we update the matrix from the manipulator data
    property( "Scale" ) = XVector2D( getManipulator( 0 ).scale() );
    property( "Translate" ) = XVector2D( getManipulator( 0 ).position() );
    }

void scaleNode::initiateManipulators( )
    {
    // initiate manipulators sets the manipulators up from the property
    Manipulator manip( property( "Scale" )->toVector2D().x(), property( "Scale" )->toVector2D().y() );
    manip.setPosition( property( "Translate" )->toVector2D().toPointF() );
    setManipulator( 0,  manip );
    }

translateNode::translateNode()
    {
    INIT_NODE

    // add the translate attributes
    addAttribute( "Translate", XVector2D(), propertyInfo::Component2D, "Translate" );

    // add a manipulator
    addManipulator( Manipulator( QPointF( 0, 0 ) ) );

    property( "longHelp" ) = "A Scale node allows the user to move the image relative to the origin.";
    property( "shortHelp" ) = "A Scale node allows the user to move the image relative to the origin.";
    }

void translateNode::updateTransform( APropertyDevice *dev, AProperty *prop )
    {
    transformNode::updateTransform( dev, prop );
    if( prop->name() == "Translate" )
        {
        // update the transform property from the translate attibutes
        QMatrix3x3 mat;
        mat(2,0) = property( "Translate" )->toVector2D().x();
        mat(2,1) = property( "Translate" )->toVector2D().y();
        property("transform") = mat;
        }
    }

void translateNode::manipulatorChanged( int )
    {
    // set the properties from the manipulator
    property( "Translate" ) = XVector2D( getManipulator( 0 ).position() );
    }

void translateNode::initiateManipulators( )
    {
    // set the manipulator up from the properties
    setManipulator( 0, Manipulator( property( "Translate" )->toVector2D().toPointF() ) );
    }

rotateNode::rotateNode()
    {
    INIT_NODE
    // add the rotate attributes
    addAttribute("angle", 0.0, propertyInfo::Double, "Angle" );
    addAttribute( "Translate", XVector2D(), propertyInfo::Component2D, "Translate" );

    // add a manipulator
    addManipulator( Manipulator( 0.0 ) );

    property( "longHelp" ) = "A Scale node allows the user to rotate the image about an arbitrary point.";
    property( "shortHelp" ) = "A Scale node allows the user to rotate the image about an arbitrary point.";
    }

void rotateNode::updateTransform( APropertyDevice *dev, AProperty *prop )
    {
    transformNode::updateTransform( dev, prop );
    if( prop->name() == "angle" || prop->name() == "Translate" )
        {
        // we need to update the transform from the angle
        double angle( property("angle")->toDouble() );
        QMatrix3x3 mat, preMat, postMat;
        mat(0,0) = cos(angle);
        mat(0,1) = -sin(angle);
        mat(1,0) = -mat(0,1);
        mat(1,1) = mat(0,0);
        preMat(2,0) = property( "Translate" )->toVector2D().x();
        preMat(2,1) = property( "Translate" )->toVector2D().y();
        postMat(2,0) = -property( "Translate" )->toVector2D().x();
        postMat(2,1) = -property( "Translate" )->toVector2D().y();
        property("transform") = postMat * mat * preMat;
        }
    }

void rotateNode::manipulatorChanged( int )
    {
    // set the properties from the manipulator
    property( "angle" ) = getManipulator( 0 ).angle();
    property( "Translate" ) = XVector2D( getManipulator( 0 ).position() );
    }

void rotateNode::initiateManipulators( )
    {
    // set the manipulator from the properties
    Manipulator manip( property( "angle" )->toDouble() );
    manip.setPosition( property( "Translate" )->toVector2D().toPointF() );
    setManipulator( 0,  manip );
    }
