#include "nodeEntity.h"
#include "nodeDB.h"
#include "nodeEntityManipulator.h"
#include "QRectF"
#include "node.h"
#include "colourPickerWidget.h"
#include "QSettings"
#include "QBitArray"

int nodeEntity::getTextureUnit()
    {
    QSettings settings;
    QBitArray freeUnits( settings.value("freeTextureUnits").toBitArray() );
    freeUnits.resize( 32 );

    // grab a unit from the texture id stack
    int _unit = -1;
    for( int x=0; x<32; x++ )
        {
        // if this unit is clear
        if( freeUnits[x] == false )
            {
            freeUnits[x] = true;
            _unit = x;
            break;
            }
        }

    settings.setValue( "freeTextureUnits", freeUnits );
    return _unit;
    }

void nodeEntity::returnTextureUnit( int _unit )
    {
    QSettings settings;
    QBitArray freeUnits( settings.value("freeTextureUnits").toBitArray() );
    freeUnits.resize( 32 );
    // restore the id
    freeUnits[_unit] = false;
    settings.setValue( "freeTextureUnits", freeUnits );
    }

inline QString glslString( QString in )
    {
    // make a nice variable name from out node name
    return in.replace( " ", "_" ).replace( "input", "in_ut").replace("-", "_");
    }

QString nodeEntity::tidyName( ) const
    {
    // return the nice name of this node
    return glslString( name() );
    }

nodeEntity::nodeEntity() : _icon( 0 )
    {
    _oldName = name();
    _sourceUnparsed = true;
    // by default we arent drawn or renderable
    setExportRenderer( 0 );
    setDrawNode( 0 );

    addProperty( "initiated", false, propertyInfo::Bool );
    property("initiated").hide();

    addProperty( "editable", false, propertyInfo::Bool );
    property("editable").hide();

    // hide directory property
    property( "entity/directory" ).hide();

    // add a isNode property for safe casting and searching the array
    addProperty( "nodeEntity/isNode", TRUE, propertyInfo::Bool );
    property( "nodeEntity/isNode" ).hide();

    // add a position for renderering, and a colour too
    addProperty( "nodeEntity/pos", XVector3D( 0.0, 0.0, 0.0 ), propertyInfo::Position, "Position" );
    property( "nodeEntity/pos" ).hide();
    addProperty( "nodeEntity/colour", XColour( 0.0, 1.0, 1.0 ), propertyInfo::ColourNoAlpha, "Colour" );
    property( "nodeEntity/colour" ).hide();

    // this property is a dummy to allow searching to connected nodes etc.
    addProperty( "output", "", propertyInfo::String );
    property( "output" ).attribute("Linkable") = TRUE;
    property( "output" ).hide();

    // this holds the type name of this node type
    addProperty( "nodeEntity/type", "", propertyInfo::String );
    property( "nodeEntity/type" ).hide();

    // thie holds the group name of this node type
    addProperty( "nodeEntity/group", "", propertyInfo::String );
    property( "nodeEntity/group" ).hide();

    // this holds the GLSL source of the node
    addProperty( "nodeEntity/source", "return vec4( 0.0 );", propertyInfo::String );
    property( "nodeEntity/source" ).hide();

    // this holds whether this property should be drawn as a view node
    addProperty( "isViewNode", FALSE, propertyInfo::Int );
    property( "isViewNode" ).hide();

    // whether it should always show its inputs when drawn
    addProperty( "alwaysShowInputs", FALSE, propertyInfo::Bool );
    property( "alwaysShowInputs" ).hide();

    // if it should be bypasses
    addProperty( "bypass", FALSE, propertyInfo::Bool, "Bypass" );

    // if its properties are open
    addProperty( "showProperties", FALSE, propertyInfo::Bool );
    property( "showProperties" ).hide();

    addProperty( "propertiesState", 0, propertyInfo::Int );
    property( "propertiesState" ).hide();


    addProperty( "longHelp", "No Help Available", propertyInfo::LongString );
    property( "longHelp" ).hide();
    addProperty( "shortHelp", "No Quick Help Available", propertyInfo::LongString );
    property( "shortHelp" ).hide();

    // connect the entity to the attributechanged signal
    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChanged(APropertyDevice*,AProperty*)) );
    QObject::connect( this, SIGNAL(propertiesChanged()), this, SLOT(propertiesChanged()) );
    }

nodeEntity::~nodeEntity( )
    {
    //delete all attributes
    while( _attributes.size() )
        {
        delete _attributes.take( _attributes.keys().front() ).var;
        }
    // and if i have an icon, delete it
    if( _icon )
        {
        delete _icon;
        }
    }

void nodeEntity::setThumbnail( QImage im )
    {
    _thumbnail = QPixmap::fromImage( im.scaledToWidth( 100, Qt::FastTransformation ) );
    if( drawNode() )
        {
        drawNode()->update();
        }
    }

void nodeEntity::addInput( QString n, QString d, bool def )
    {
    // add a property for the input first
    addProperty( n, "", propertyInfo::String, d );
    AProperty &prop( property( n ) );
    // mark it as linkable and possible as the default ( used for byapssing )
    prop.attribute("Linkable") = TRUE;
    prop.attribute("default") = def;
    // hide it
    prop.hide();
    }

QToolBar *nodeEntity::toolbar()
    {
    return 0;
    }

bool nodeEntity::canUse( QMouseEvent * )
    {
    return false;
    }

void nodeEntity::use( QMouseEvent * )
    {
    }

void nodeEntity::entityChanged( APropertyDevice *, AProperty *in )
    {
    if( in->name() == "nodeEntity/source" )
        {
        _sourceUnparsed = true;
        }
    if( in->attributes().contains("Attribute") )
        {
        // set the GLSL variable if the GUI changes
        switch( in->type() )
            {
            case propertyInfo::Double:
                attribute <jLib::graphics::shader::sFloat *>( in->name() )->set( in->value().toDouble() );
                break;
            case calcPropertyInfo::Colour:
                attribute <jLib::graphics::shader::sColour *>( in->name() )->set( in->value().toColour() );
                break;
            case propertyInfo::Component2D:
                attribute <jLib::graphics::shader::sVec2 *>( in->name() )->set( in->value().toVector2D() );
                break;
            case propertyInfo::Component3D:
                attribute <jLib::graphics::shader::sVec3 *>( in->name() )->set( in->value().toVector3D() );
                break;
            case propertyInfo::Component4D:
                attribute <jLib::graphics::shader::sVec4 *>( in->name() )->set( in->value().toVector4D() );
                break;
            case propertyInfo::Matrix3x3:
                attribute <jLib::graphics::shader::sMat3x3 *>( in->name() )->set( in->value().toMatrix3x3() );
                break;
            default:
                jError << "That type cant be used as an attribute - " << in->type() << endl;
                break;
            };
        }
    }

void nodeEntity::addAttribute( QString n, AVariant v, int t, QString d )
    {
    // add a property first
    addProperty( n, v, t, d );
    AProperty &prop( property( n ) );
    // mark it as attribute

    // then create a GLSL variable for it!
    QString attrName( glslString( name()+"_"+n ) );
    switch( t )
        {
        case propertyInfo::Double:
            _attributes.insert( n, shaderVar( new jLib::graphics::shader::sFloat( attrName.toStdString() ), t ) );
            attribute <jLib::graphics::shader::sFloat *>( n )->set( v.toDouble() );
            break;
        case calcPropertyInfo::Colour:
            _attributes.insert( n, shaderVar( new jLib::graphics::shader::sColour( attrName.toStdString() ), t ) );
            attribute <jLib::graphics::shader::sColour *>( n )->set( v.toColour() );
            break;
        case propertyInfo::Component2D:
            _attributes.insert( n, shaderVar( new jLib::graphics::shader::sVec2( attrName.toStdString() ), t ) );
            attribute <jLib::graphics::shader::sVec2 *>( n )->set( v.toVector2D() );
            break;
        case propertyInfo::Component3D:
            _attributes.insert( n, shaderVar( new jLib::graphics::shader::sVec3( attrName.toStdString() ), t ) );
            attribute <jLib::graphics::shader::sVec3 *>( n )->set( v.toVector3D() );
            break;
        case propertyInfo::Component4D:
            _attributes.insert( n, shaderVar( new jLib::graphics::shader::sVec4( attrName.toStdString() ), t ) );
            attribute <jLib::graphics::shader::sVec4 *>( n )->set( v.toVector4D() );
            break;
        case propertyInfo::Matrix3x3:
            _attributes.insert( n, shaderVar( new jLib::graphics::shader::sMat3x3( attrName.toStdString() ), t ) );
            attribute <jLib::graphics::shader::sMat3x3 *>( n )->set( v.toMatrix3x3() );
            break;
        default:
            jError << "That type cant be used as an attribute - " << t << endl;
            break;
        };
    prop.attribute("Attribute") = TRUE;
    }

void nodeEntity::propertiesChanged()
    {
    bool rebuild = false;
    foreach( QString n, _attributes.keys() )
        {
        if( !propertyExists( n ) )
            {
            delete _attributes.value( n ).var;
            _attributes.remove( n );
            rebuild = true;
            }
        }
    if( rebuild )
        {
        emit needsRebuild();
        }
    _sourceUnparsed = true;
    }

void nodeEntity::setColour( float r, float g, float b )
    {
    property( "nodeEntity/colour" ) = XColour( r, g, b, 1.0 );
    }

void nodeEntity::disconnect( AEntity *output )
    {
    // disconnect this node from the output node
    FOREACHCONST( properties(), p )
        {
        AAttribute it = (*p)->attribute("nodeConnectionPath");
        if( !it->isNull() && AEntityLocation( parent(), it->toString() ) == output->location().path() )
            {
            property( p.key() ).attribute("nodeConnectionPath") = AVariant();
            property( p.key() ).attribute("nodeConnectionProperty") = AVariant();
            property( p.key() ).removeAttribute( "singleInput" );
            }
        }
    }

void nodeEntity::requestSetViewNode( int in )
    {
    // we cant do this ourselves, so we emit a signal and hope there is someone lisetening
    emit _requestSetViewNode( this, in );
    }

propertyList nodeEntity::connectedInputs()
    {
    // find the inputs that are linkable and return them
    propertyList ret;
    foreach( AProperty *p, properties() )
        {
        if( p->attributes().contains("Linkable") && p->attributes().contains( "singleInput" ) )
            {
            ret << p;
            }
        }
    return ret;
    }

propertyListConst nodeEntity::connectedInputs() const
    {
    // find the inputs that are linkable and return them
    propertyListConst ret;
    foreach( const AProperty *p, properties() )
        {
        if( p->attributes().contains("Linkable") && p->attributes().contains( "singleInput" ) )
            {
            ret << p;
            }
        }
    return ret;
    }

nodeEntityList nodeEntity::connectedNodes( QString in )
    {
    return connectedNodes( property( in ) );
    }

nodeEntityListConst nodeEntity::connectedNodes( QString in ) const
    {
    return connectedNodes( property( in ) );
    }

nodeEntityList nodeEntity::connectedNodes( const AProperty &in )
    {
    // test if we are an input or an output, if input call the singular function then put it in a list an return it
    if( in.attributes().contains( "singleInput" ) )
        {
        nodeEntityList ret;
        ret << connectedNode( in );
        return ret;
        }

    nodeEntityList ret;

    // search for nodes in the array
    AEntitySearch search( array()->search().findHasProperty( "nodeEntity/isNode" ) );

    foreach( AEntity *s, search )
        {
        foreach( AProperty *p, s->properties() )
            {
            // if this property is linkable and is an input
            if( p->attributes().contains("Linkable") && p->attributes().contains( "singleInput" ) )
                {
                // and is connected to me, add it to the return list
                if( AEntityLocation( s->parent(), p->attributes().value("nodeConnectionPath").toString() ) == location() &&
                    p->attributes().value("nodeConnectionProperty").toString() == in.name() )
                    {
                    ret << static_cast <nodeEntity *> (s);
                    }
                }
            }
        }

    return ret;
    }

nodeEntityListConst nodeEntity::connectedNodes( const AProperty &in ) const
    {
    // test if we are an input or an output, if input call the singular function then put it in a list an return it
    if( in.attributes().contains( "singleInput" ) )
        {
        nodeEntityListConst ret;
        ret << connectedNode( in );
        return ret;
        }

    nodeEntityListConst ret;

    // search for nodes in the array
    AEntitySearchConst search( *array() );

    foreach( const AEntity *s, search )
        {
        foreach( const AProperty *p, s->properties() )
            {
            // if this property is linkable and is an input
            if( p->attributes().contains("Linkable") && p->attributes().contains( "singleInput" ) )
                {
                // and is connected to me, add it to the return list
                if( AEntityLocation( s->parent(), p->attributes().value("nodeConnectionPath").toString() ) == location() &&
                    p->attributes().value("nodeConnectionProperty").toString() == in.name() )
                    {
                    ret << static_cast <const nodeEntity *> (s);
                    }
                }
            }
        }

    return ret;
    }

void nodeEntity::disconnect( )
    {
    // disconnect this node completely, first find things connected to input
    propertyList list( connectedProperties("output") );
    foreach( AProperty *p, list )
        {
        p->removeAttribute("nodeConnectionPath");
        p->removeAttribute("nodeConnectionProperty");
        p->removeAttribute( "singleInput" );
        }

    // then disconnect output.
    propertyList list2( connectedInputs() );
    foreach( AProperty *p, list2 )
        {
        p->removeAttribute("nodeConnectionPath");
        p->removeAttribute("nodeConnectionProperty");
        p->removeAttribute( "singleInput" );
        }
    }

void nodeEntity::disconnect( QString inPropName, AEntity *output, QString outPropName )
    {
    // disconnect a specific property
    AProperty &outProp( output->property( outPropName ) );
    AProperty &inProp( property( inPropName ) );
    inProp.removeAttribute("nodeConnectionPath");
    inProp.removeAttribute("nodeConnectionProperty");
    inProp.removeAttribute( "singleInput" );
    foreach( AProperty *it, _connections.keys() )
        {
        if( it == &inProp && _connections.value( it ) == &outProp )
            {
            _connections.remove( it );
            }
        }
    emit onDisconnection( this, &inProp );
    emit onDisconnection( output, &outProp );
    }

void nodeEntity::prePropertyChanged( AProperty *prop )
    {
    if( prop->name() == "entity/name" )
        {
        AEntitySearch search( array()->search().findHasProperty( "nodeEntity/isNode" ) );

        foreach( AEntity *ent, search )
            {
            foreach( AProperty *prop, ent->properties( ) )
                {
                if( prop->attributes().contains("singleInput") &&
                    prop->attributes()["nodeConnectionPath"].toString() == _oldName )
                    {
                    prop->attribute("nodeConnectionPath") = name();
                    }
                }
            }
        _oldName = name();
        }
    else if( prop->attributes().contains("Linkable") )
        {
        emit requestNetworkRedraw();
        }
    AEntity::prePropertyChanged( prop );
    }

void nodeEntity::connect( QString inPropName, nodeEntity *output, QString outPropName )
    {
    if( !isOutputLooselyConnected( output ) )
        {
        // connect two properties
        AProperty &outProp( output->property( outPropName ) );
        AProperty &inProp( property( inPropName ) );

        _connections.insert( &inProp, &outProp );

        inProp.attribute("singleInput") = TRUE;
        inProp.attribute("nodeConnectionPath") = output->name();
        inProp.attribute("nodeConnectionProperty") = outPropName;
        emit onConnection( output, &outProp );

        emit output->onConnection( this, &inProp );
        }
    else
        {
        AWarning << "Cyclic connections aren't supported.";
        }
    }

nodeEntity *nodeEntity::connectedNode( const AProperty &prop )
    {
    // find the node connected to a property
    if( prop.attributes().contains( "nodeConnectionPath" ) &&
        prop.attributes().contains( "nodeConnectionProperty" ) &&
            prop.attribute( "nodeConnectionPath")->toString() != "" &&
            prop.attribute( "nodeConnectionProperty")->toString() != "" )
        {
        AEntitySearch connected( array()->search().findEntity( AEntityLocation( parent(), prop.attribute( "nodeConnectionPath")->toString() ) ) );
        if( connected.size() )
            {
            // we can safely cast as it has has been connected
            return static_cast <nodeEntity *> (&(connected.front()));
            }
        }
    return 0;
    }

const nodeEntity *nodeEntity::connectedNode( const AProperty &prop ) const
    {
    // find the node connected to a property
    if( prop.attributes().contains( "nodeConnectionPath" ) &&
            prop.attributes().contains( "nodeConnectionProperty" ) &&
            prop.attribute( "nodeConnectionPath" )->toString() != "" &&
            prop.attribute( "nodeConnectionProperty")->toString() != "" )
        {
        AEntitySearch connected( array()->search().findEntity( AEntityLocation( parent(), prop.attribute( "nodeConnectionPath")->toString() ) ) );
        if( connected.size() )
            {
            // we can safely cast as it has has been connected
            return static_cast <nodeEntity *> (&(connected.front()));
            }
        }
    return 0;
    }

nodeEntity *nodeEntity::connectedNode( QString in )
    {
    // if the property exists
    if( propertyExists( in ) )
        {
        // find its connected node
        AProperty &prop( property( in ) );
        return connectedNode( prop );
        }
    return 0;
    }

const nodeEntity *nodeEntity::connectedNode( QString in ) const
    {
    // if the property exists
    if( propertyExists( in ) )
        {
        // find its connected node
        const AProperty &prop( property( in ) );
        return connectedNode( prop );
        }
    return 0;
    }

propertyList nodeEntity::connectedProperties( QString in )
    {
    return connectedProperties( property( in ) );
    }

propertyListConst nodeEntity::connectedProperties( QString in ) const
    {
    return connectedProperties( property( in ) );
    }

propertyList nodeEntity::connectedProperties( const AProperty &in )
    {
    // if the property is an input, then find the result and put it in a list
    if( in.attributes().contains( "singleInput" ) )
        {
        propertyList ret;
        ret << &connectedProperty( in );
        return ret;
        }

    propertyList ret;

    AEntitySearch search( *array() );

    foreach( AEntity *s, search )
        {
        foreach( AProperty *p, s->properties() )
            {
            // if this property is connectable and is connected
            if( p->attributes().contains("Linkable") && p->attributes().contains( "singleInput" ) )
                {
                // and is connected to me
                if( AEntityLocation( parent(), p->attributes().value("nodeConnectionPath").toString() ) == location() &&
                    p->attributes().value("nodeConnectionProperty").toString() == in.name() )
                    {
                    // add to ouptput list
                    ret << p;
                    }
                }
            }
        }

    return ret;
    }

propertyListConst nodeEntity::connectedProperties( const AProperty &in ) const
    {
    // if the property is an input, then find the result and put it in a list
    if( in.attributes().contains( "singleInput" ) )
        {
        propertyListConst ret;
        ret << &connectedProperty( in );
        return ret;
        }

    propertyListConst ret;

    AEntitySearchConst search( *array() );

    foreach( const AEntity *s, search )
        {
        foreach( const AProperty *p, s->properties() )
            {
            // if this property is connectable and is connected
            if( p->attributes().contains("Linkable") && p->attributes().contains( "singleInput" ) )
                {
                // and is connected to me
                if( AEntityLocation( parent(), p->attributes().value("nodeConnectionPath").toString() ) == location() &&
                    p->attributes().value("nodeConnectionProperty").toString() == in.name() )
                    {
                    // add to ouptput list
                    ret << p;
                    }
                }
            }
        }

    return ret;
    }

AProperty &nodeEntity::connectedProperty( const AProperty &prop )
    {
    nodeEntity *ent( connectedNode( prop ) );
    return ent->property( prop.attribute( "nodeConnectionProperty" )->toString() );
    }

const AProperty &nodeEntity::connectedProperty( const AProperty &prop ) const
    {
    const nodeEntity *ent( connectedNode( prop ) );
    return ent->property( prop.attribute( "nodeConnectionProperty" )->toString() );
    }

AProperty &nodeEntity::connectedProperty( QString prop )
    {
    nodeEntity *ent( connectedNode( prop ) );
    return ent->property( property( prop ).attribute( "nodeConnectionProperty" )->toString() );
    }

const AProperty &nodeEntity::connectedProperty( QString prop ) const
    {
    const nodeEntity *ent( connectedNode( prop ) );
    return ent->property( property( prop ).attribute( "nodeConnectionProperty" )->toString() );
    }

void nodeEntity::dislocate( QHash<AEntityLocation, AEntityLocation> &objMap )
    {
    AEntity::dislocate( objMap );
    property( "isViewNode" ) = false;

    // take this node and make it "unconnected" so it can be copied and pasted without weird connections appearing...
    propertyList list2( connectedInputs() );
    foreach( AProperty *p, list2 )
        {
        AEntityLocation oldConnectionPath( objMap.key( location() ).parent(), p->attribute("nodeConnectionPath")->toString() );
        if( objMap.contains( oldConnectionPath ) )
            {
            p->attribute("nodeConnectionPath") = objMap[ oldConnectionPath ].name();
            }
        else
            {
            p->removeAttribute("nodeConnectionPath");
            p->removeAttribute("nodeConnectionProperty");
            }
        }
    }

void nodeEntity::relocated()
    {
    _oldName = name();
    AEntity::relocated();
    // when we are relocated, we need to connect properties to our nodes we find etc.
    FOREACHCONST( properties(), p )
        {
        AProperty::attributeMap::const_iterator it = (*p)->attributes().find( "nodeConnectionPath" );
        AProperty::attributeMap::const_iterator it2 = (*p)->attributes().find( "nodeConnectionProperty" );
        if( it != (*p)->attributes().end() && it2 != (*p)->attributes().end() )
            {
            AEntitySearch con = array()->search().findEntity( AEntityLocation( parent(), it->toString() ) );
            if( con.size() && con[0].propertyExists( it2->toString() ) )
                {
                connect( p.key(), static_cast <nodeEntity *> (&(con[0])), it2->toString() );
                }
            else if( it->toString() != "" && it2->toString() != "" )
                {
                ALog<<"Couldn't reconnect property '"<<it->toString()<<" -> "<<it2->toString()<<"'"<<endl;
                }
            }
        // we also need to recreate the GLSL variables...
        if( (*p)->attributes().contains("Attribute") )
            {
            QString attrName( glslString( name()+"_"+(*p)->name() ) );
            switch( (*p)->type() )
                {
                case propertyInfo::Double:
                    _attributes.insert( (*p)->name(), shaderVar( new jLib::graphics::shader::sFloat( attrName.toStdString() ), (*p)->type() ) );
                    attribute <jLib::graphics::shader::sFloat *>( (*p)->name() )->set( (*p)->value().toDouble() );
                    break;
                case calcPropertyInfo::Colour:
                    _attributes.insert( (*p)->name(), shaderVar( new jLib::graphics::shader::sColour( attrName.toStdString() ), (*p)->type() ) );
                    attribute <jLib::graphics::shader::sColour *>( (*p)->name() )->set( (*p)->value().toColour() );
                    break;
                case propertyInfo::Component2D:
                    _attributes.insert( (*p)->name(), shaderVar( new jLib::graphics::shader::sVec2( attrName.toStdString() ), (*p)->type() ) );
                    attribute <jLib::graphics::shader::sVec2 *>( (*p)->name() )->set( (*p)->value().toVector2D() );
                    break;
                case propertyInfo::Component3D:
                    _attributes.insert( (*p)->name(), shaderVar( new jLib::graphics::shader::sVec3( attrName.toStdString() ), (*p)->type() ) );
                    attribute <jLib::graphics::shader::sVec3 *>( (*p)->name() )->set( (*p)->value().toVector3D() );
                    break;
                case propertyInfo::Component4D:
                    _attributes.insert( (*p)->name(), shaderVar( new jLib::graphics::shader::sVec4( attrName.toStdString() ), (*p)->type() ) );
                    attribute <jLib::graphics::shader::sVec4 *>( (*p)->name() )->set( (*p)->value().toVector4D() );
                    break;
                case propertyInfo::Matrix3x3:
                    _attributes.insert( (*p)->name(), shaderVar( new jLib::graphics::shader::sMat3x3( attrName.toStdString() ), (*p)->type() ) );
                    attribute <jLib::graphics::shader::sMat3x3 *>( (*p)->name() )->set( (*p)->value().toMatrix3x3() );
                    break;
                default:
                    jError << "That type cant be used as an attribute - " << (*p)->type() << endl;
                    break;
                };
            }
        }
    _sourceUnparsed = true;
    }

void nodeEntity::initiate()
    {
    _sourceUnparsed = true;
    }

void nodeEntity::paint( QPainter *pai )
    {
    foreach( Manipulator manip, manipulators() )
        {
        if( manip.viewScale().x() > 0 && manip.viewScale().x() < HUGE_VAL )
            {
            manip.paint( pai );
            }
        }
    }

void nodeEntity::prepareToRender( )
    {
    }

void nodeEntity::prepareToRebuild( )
    {
    _sourceUnparsed = true;
    }

bool nodeEntity::canInline( QString *src ) const
    {
    QString newSrc( parsedSource() );
    if( src )
        {
        *src = parsedSource();
        }
    else
        {
        src = &newSrc;
        }

    QRegExp start( "\\s*return\\s*" );
    QRegExp end( ";\\s*" );

    int startFind = start.indexIn( *src );
    int endFind = end.lastIndexIn( *src );

    QString ret( "( " + src->mid( start.matchedLength(), endFind - start.matchedLength() ) + " )" );

    if( startFind == 0 && endFind == src->length() - 1 )
        {
        *src = ret;
        return true;
        }

    return false;
    }

QString nodeEntity::call( QString coords ) const
    {
    // find the code to call this node, or if bypassing, find the code to call the node above this node...
    if( property( "bypass" )->toBool() == FALSE )
        {
        if( propertyExists( "callPointer" ) && property( "callPointer" )->toString() != "" )
            {
            AEntitySearch search( array()->search().findEntity( AEntityLocation( location(), property( "callPointer" ).value().toString() ) ) );
            if( search.size() && search[0].castTo<nodeEntity*>() )
                {
                return search[0].castTo<nodeEntity*>()->call( coords );
                }
            }
        else
            {
#ifdef SYNAPSE_GLSL_INLINE
            QString src;
            if( canInline( &src ) )
                {
                return src;
                }
#endif
            return tidyName()+"_fn( " + coords + " )";
            }
        }
    else
        {
        const nodeEntity *use = 0;
        foreach( AProperty *prop, properties() )
            {
            const nodeEntity *tempUse = connectedNode( *prop );
            if( tempUse != 0 )
                {
                use = tempUse;
                }
            if( prop->attribute("default")->toBool() == TRUE && tempUse )
                {
                break;
                }
            }
        if( use )
            {
            return use->call( coords );
            }
        }
    // otherwise return black.
    return "vec4( 0.0 );";
    }

QString nodeEntity::parseInput( QString prop, QString coord ) const
    {
    // try to call the connected node, or black
    const nodeEntity *cN( connectedNode( prop ) );
    if( cN )
        {
        return cN->call( coord );
        }

    if( propertyExists( prop ) && property(prop).attributes().contains( "inputForwardAttribute" ) )
        {
        AEntitySearch search( array()->search().findEntity( parent() ) );
        if( search.size() )
            {
            nodeEntity *node( search[0].castTo<nodeEntity*>() );
            if( node )
                {
                nodeEntity *connectedNode( node->connectedNode( property(prop).attribute( "inputForwardAttribute" )->toString() ) );
                if( connectedNode )
                    {
                    return connectedNode->call( coord );
                    }
                }
            }
        }

    return "vec4( 0.0 )";
    }

QString nodeEntity::parseAttribute( QString in ) const
    {
    // the attribute name must be nice, and unique too...
    return glslString( name() + "_" + in );
    }

QString nodeEntity::parsedSource( ) const
    {
    if( _sourceUnparsed )
        {
        reparseSource();
        }
    return _pSource;
    }

void nodeEntity::reparseSource( ) const
    {
    // grab the source from the property
    QString src( property("nodeEntity/source")->toString() );
    QString result;

    //Parse attributes
        {
        // strip out attribute requests
        QRegExp regExp( "getAttribute\\(\\s*\\\"([0-9A-Za-z _-]*)\\\"\\s*\\)" );
        // we want minimal finds
        regExp.setMinimal( TRUE );

        int pos = 0;
        int lastPos = 0;
        while (pos >= 0)
            {
            pos = regExp.indexIn( src, pos );
            if ( pos >= 0 )
                {
                // we place the parsed attribute value where the call to getAttribute was.
                result += src.mid( lastPos, pos - lastPos ) + parseAttribute( regExp.cap( 1 ) );
                pos += regExp.matchedLength();
                lastPos = pos;
                }
            }

        result += src.mid( lastPos );
        }

    // swap the src and result, and parse again
    src = result;
    result = "";

    //Parse inputs
        {
        // there is a problem with parsing this call if you use brackets inside the function call... we should look at this
        QRegExp regExp( "getInput\\(\\s*\\\"([0-9A-Za-z ]*)\\\"\\s*\\,\\s*(.*)\\s*\\)" );
        regExp.setMinimal( TRUE );

        int pos = 0;
        int lastPos = 0;
        while (pos >= 0)
            {
            pos = regExp.indexIn( src, pos );
            if ( pos >= 0 )
                {
                result += src.mid( lastPos, pos - lastPos ) + parseInput( regExp.cap( 1 ), regExp.cap( 2 ) );
                pos += regExp.matchedLength();
                lastPos = pos;
                }
            }

        result += src.mid( lastPos );
        }

    // return parsed result
    _pSource = result;
    }

QRectF nodeEntity::rect()
    {
    // check for output forwarding
    if( propertyExists( "callPointer" ) )
        {
        AEntitySearch search( array()->search().findEntity( AEntityLocation( location(), property( "callPointer" ).value().toString() ) ) );
        if( search.size() && search[0].castTo<nodeEntity*>() && search[0].castTo<nodeEntity*>() != this )
            {
            return search[0].castTo<nodeEntity*>()->rect();
            }
        return( QRectF( 0, 0, 1, 1 ) );
        }

    // unite all the input rectangles and return
    QRectF ret;
    foreach( AProperty *prop, properties() )
        {
        if( prop->attributes().contains("Linkable") )
            {
            nodeEntity *node( connectedNode( *prop ) );
            if( node )
                {
                ret = ret.united( node->rect() );
                }
            else if( prop->attributes().contains( "inputForwardAttribute" ) )
                {
                AEntitySearch search( array()->search().findEntity( parent() ) );
                if( search.size() )
                    {
                    nodeEntity *node( search[0].castTo<nodeEntity*>() );
                    if( node )
                        {
                        nodeEntity *connectedNode( node->connectedNode( prop->attribute( "inputForwardAttribute" )->toString() ) );
                        if( connectedNode )
                            {
                            ret = ret.united( connectedNode->rect() );
                            }
                        }
                    }
                }
            }
        }
    // or return a default rectangle
    if( ret.isNull() )
        {
        return( QRectF( 0, 0, 1, 1 ) );
        }
    return ret;
    }

nodeEntityList nodeEntity::renderDependantNodeNetwork()
    {
    // find all nodes connect upstream of this one
    nodeEntityList ret;

    ret << this;

    // use call forwarding if enabled
    if( propertyExists( "callPointer" ) && property( "callPointer" )->toString() != "" )
        {
        AEntitySearch search( array()->search().findEntity( AEntityLocation( location(), property( "callPointer" ).value().toString() ) ) );
        if( search.size() && search[0].castTo<nodeEntity*>() )
            {
            ret << search[0].castTo<nodeEntity*>()->renderDependantNodeNetwork();
            }
        }
    else if( !propertyExists( "independantRenderingToParent" ) )
        {
        foreach( AProperty *prop, properties() )
            {
            if( prop->attributes().contains("Linkable") )
                {
                nodeEntity *cN( connectedNode( *prop ) );

                if( cN )
                    {
                    ret << cN->renderDependantNodeNetwork();
                    }
                else if( array() && prop->attributes().contains( "inputForwardAttribute" ) )
                    {
                    AEntitySearch search( array()->search().findEntity( parent() ) );
                    if( search.size() )
                        {
                        nodeEntity *node( search[0].castTo<nodeEntity*>() );
                        if( node )
                            {
                            ret << node;

                            nodeEntity *connectedNode( node->connectedNode( prop->attribute( "inputForwardAttribute" )->toString() ) );
                            if( connectedNode )
                                {
                                ret << connectedNode->renderDependantNodeNetwork();
                                }
                            }
                        }
                    }
                }
            }
        }
    return ret;
    }

QIcon nodeEntity::icon()
    {
    // try to create an icon for this node
    if( _icon == 0 )
        {
        _icon = new QIcon( nodeDB::nodeType( property( "nodeEntity/group" )->toString(), property( "nodeEntity/type" )->toString(), property( "nodeEntity/colour" )->toColour().toLDRColour() ).icon() );
        }
    return *_icon;
    }

void nodeEntity::definitions( QHash <QString, QString> &functions ) const
    {
    // grab the definitions from this node, basically strings with code needed by this node in.
    QString tName( tidyName() );
    if( !functions.contains( tName+"_fn" ) )
        {
#ifdef SYNAPSE_GLSL_INLINE
        if( !canInline( ) )
#endif
            {
            functions.insert( tName+"_fn", parsedSource() );
            }
        FOREACHCONST( _attributes, a )
            {
            QString newName( glslString( tName + "_" + a.key() ) );
            a->var->setName( newName.toStdString() );
            }
        }
    }

void nodeEntity::addVariables( jLib::graphics::shader::variableSet &set )
    {
    // Add variables
        {
        FOREACH( _attributes, a )
            {
            set.add( *a->var );
            }
        }
    }

int nodeEntity::viewNode() const
    {
    return property( "isViewNode" )->toInt();
    }

void nodeEntity::setViewNode( int in )
    {
    int old( property( "isViewNode" )->toBool() );
    property( "isViewNode" ) = in;
    if( in != old )
        {
        emit isViewNodeChanged();
        }
    }

int nodeEntity::addManipulator( const nodeEntity::Manipulator &manip )
    {
    // add a manipulator and return id ofit
    XGetPropertyMember( manipulators ) << manip;
    return XGetPropertyMember( manipulators ).size() - 1;
    }

void nodeEntity::removeManipulator( int x )
    {
    if( XGetPropertyMember( manipulators ).size() > x )
        {
        XGetPropertyMember( manipulators ).removeAt( x );
        }
    }

void nodeEntity::clearManipulators( )
    {
    XGetPropertyMember( manipulators ).clear();
    }

const nodeEntity::Manipulator &nodeEntity::getManipulator( int x ) const
    {
    return XGetPropertyMember( manipulators )[x];
    }

void nodeEntity::setManipulator( int x, const nodeEntity::Manipulator &in )
    {
    XGetPropertyMember( manipulators )[x] = in;
    }

void nodeEntity::manipulatorChanged( int )
    {
    }

void nodeEntity::initiateManipulators( )
    {
    }

void nodeEntity::setTime( int frame )
    {
    //pass the message up the chain, until another function reimplements the function
    propertyList list( connectedInputs() );

    foreach( AProperty *prop, list )
        {
        nodeEntity *cN( connectedNode( *prop ) );
        if( cN )
            {
            cN->setTime( frame );
            }
        }
    }

void nodeEntity::resetProperties()
    {
    property( "showProperties" ) = false;
    if( drawNode() )
        {
        drawNode()->setDisplayPropertiesFromEntity();
        }
    }

bool nodeEntity::isOutputLooselyConnected( nodeEntity *in )
    {
    if( this == in )
        {
        return true;
        }

    bool ret( false );
    nodeEntityList nodes( connectedNodes("output") );
    foreach( nodeEntity *node, nodes )
        {
        ret |= node->isOutputLooselyConnected( in );
        }

    return ret;
    }

QString nodeEntity::projectDirectory()
    {
    QSettings settings;
    QString dir( settings.value( "projectDirectory" ).toString() );
    if( dir != "" )
        {
        return dir;
        }
    return "";
    }

void nodeEntity::setProjectDirectory( QString str )
    {
    QSettings settings;
    settings.setValue( "projectDirectory", str );
    }
