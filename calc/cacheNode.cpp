#include "cacheNode.h"
#include "AFileDialog"
#include "QRectF"
#include "colourPickerWidget.h"

cacheNode::cacheNode() : _sampler( "null" )
    {
    INIT_NODE

    addProperty("independantRenderingToParent", true, propertyInfo::Bool );
    property("independantRenderingToParent").hide();

    addAttribute("rect", XVector4D( 0, 0, 1, 1 ), calcPropertyInfo::Colour );
    property("rect").hide();

    property("editable") = true;

    addInput( "Input", "Input", true );
    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(reconnect(APropertyDevice*,AProperty*)) );
    QObject::connect( &_renderer, SIGNAL(aspectChanged()), this, SLOT(aspectChanged()) );
    QObject::connect( this, SIGNAL(onDisconnection(APropertyDevice*,AProperty*)), this, SLOT(doUpdate()) );
    QObject::connect( this, SIGNAL(onConnection(APropertyDevice*,AProperty*)), this, SLOT(doUpdate()) );
    QObject::connect( &_renderer, SIGNAL(needsUpdate()), this, SLOT(doUpdate()) );

    // grab and check a texture unit
    _unit = getTextureUnit();
    if( _unit == -1 )
        {
        AWarning << "There are no free image units available." << endl;
        }

    property( "longHelp" ) = "A cache will render its input to texture then sample the texture as its output.\n\nThe cache operation will optimise performance for expensive nodes.";
    property( "shortHelp" ) = "A cache will render its input to texture then sample the texture as its output.\n\nThe cache operation will optimise performance for expensive nodes.";

    rebuild();
    }

void cacheNode::doUpdate()
    {
    Q_EMIT needsUpdate();
    }

cacheNode::~cacheNode()
    {
    // give texture unit back
    returnTextureUnit( _unit );
    }

void cacheNode::relocated()
    {
    // trigger a reload of the image
    nodeEntity::relocated();
    _sampler.setName( tidyName().toStdString() + "_cacheTexture" );
    }

void cacheNode::addVariables( jLib::graphics::shader::variableSet &set )
    {
    // add the sampler variable
    set.add( _sampler );
    // then normal variables
    nodeEntity::addVariables( set );
    }

void cacheNode::initiate()
    {
    }

void cacheNode::reconnect( APropertyDevice*, AProperty *prop )
    {
    nodeEntity *node( connectedNode( *prop ) );
    if( prop->name() == "Input" && _renderer.node() != node )
        {
        _renderer.setNode( node );
        }
    else if( prop->name() == "entity/name" )
        {
        _sampler.setName( tidyName().toStdString() + "_cacheTexture" );
        rebuild();
        }
    }

void cacheNode::rebuild()
    {
    property("nodeEntity/source") = "if( coords.x > getAttribute(\"rect\").x &&\n" \
        "coords.y > getAttribute(\"rect\").y &&\n" \
        "coords.x < getAttribute(\"rect\").x + getAttribute(\"rect\").z &&\n" \
        "coords.y < getAttribute(\"rect\").y + getAttribute(\"rect\").w )\n" \
        "{\n" \
        "return texture2D( " + tidyName() + "_cacheTexture, ( ( coords - getAttribute(\"rect\").xy ) / getAttribute(\"rect\").zw ) );\n" \
        "}\n" \
        "return vec4( 0.0 );\n";
    }

void cacheNode::aspectChanged()
    {
    property("rect") = XVector4D( _renderer.rect().left(), _renderer.rect().top(), _renderer.rect().width(), _renderer.rect().height() );
    }

QRectF cacheNode::rect()
    {
    return _renderer.rect();
    }

void cacheNode::setTime( int in )
    {
    _renderer.setTime( in );
    }

void cacheNode::prepareToRender()
    {
    nodeEntity::prepareToRender();
    jLib::graphics::pixel::texture &tex( _renderer.texture() );
    _sampler.set( &tex );
    tex.setTextureUnit( _unit );
    }
