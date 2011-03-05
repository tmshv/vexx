#include "abstractQImageNode.h"
#include "QRectF"
#include "QGLContext"
#include "colourPickerWidget.h"

abstractQImageNode::abstractQImageNode() : _image( 0 ), _sampler( "null" )
    {
    INIT_NODE
    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(rebuild()) );

    addAttribute( "rect", 0.0, calcPropertyInfo::Colour, "Image Rect" );
    property("rect").hide();

    // grab and check a texture unit
    _unit = getTextureUnit();
    if( _unit == -1 )
        {
        AWarning << "There are no free image units available." << endl;
        }
    }

abstractQImageNode::~abstractQImageNode()
    {
    delete _image;
    // give texture unit back
    returnTextureUnit( _unit );
    }

void abstractQImageNode::addVariables( jLib::graphics::shader::variableSet &set )
    {
    // add the sampler variable
    set.add( _sampler );
    // then normal variables
    nodeEntity::addVariables( set );
    }

void abstractQImageNode::setImage( QPointF point, QImage im )
    {
    point.setY( -point.y() );
    _rect = QRectF( point, im.size() );
    delete _image;
    // render the image to a texture and set up the sampler
    _image = new jLib::graphics::pixel::texture( im );
    _sampler.set( _image );
    property("rect") = XVector4D( _rect.left(), _rect.top(), _rect.width(), _rect.height() );

    setThumbnail( im );
    }

QRectF abstractQImageNode::rect()
    {
    return _rect;
    }

void abstractQImageNode::rebuild()
    {
    // set the name to the entity name
    _sampler.setName( tidyName().toStdString() );

    // check we have a valid image
    if( _image && _image->isValid() )
        {
        // set up the sampler
        _sampler.set( _image );
        // set the texute unit
        _image->setTextureUnit( _unit );

        // the source for actually sampling a texture
        property( "nodeEntity/source" ) = "vec2 pos = vec2( ( ( coords.s - getAttribute( \"rect\" ).x ) / getAttribute( \"rect\" ).z ), ( ( coords.t - getAttribute( \"rect\" ).y ) / getAttribute( \"rect\" ).w ) );"
           "if( pos.s >= 0.0 && pos.s <= 1.0 && \n" \
           "pos.t >= 0.0 && pos.t <= 1.0 )\n" \
           "{\n" \
           "return texture2D( " + tidyName() + ", pos );\n" \
           "}\n" \
           "else\n" \
           "{\n" \
           "return vec4( 0.0 );\n" \
           "}\n";
        }
    else
        {
        // else null
        property( "nodeEntity/source") = "return vec4( 0.0 );";
        }

    }
