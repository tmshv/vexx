#include "QDomDocument"
#include "nodeEntity.h"
#include "nodeDB.h"
#include "AFileDialog"
#include "QSettings"
#include "groupNode.h"
#include "node.h"
#include "colourPickerWidget.h"
#include "QLibrary"
#include "QApplication"

namespace nodeDB {

static QHash <AEntityType, QPair<AEntityCreatorFn,QColor> > codeNodes;

static QList <nodeTypesWatcher *> watchers;

// called to inform all node type observers of a type change
void nodeTypesChanged()
    {
    foreach( nodeTypesWatcher *obj, watchers )
        {
        // emit a signal on each
        obj->triggerTypesChanged();
        }
    }

// create a watcher with a parent
nodeTypesWatcher::nodeTypesWatcher( QObject *parent ) : QObject( parent )
    {
    watchers << this;
    }

// destory this watcher, and remove this from the informers list
nodeTypesWatcher::~nodeTypesWatcher()
    {
    watchers.removeAll( this );
    }

// emit a types changes signal
void nodeTypesWatcher::triggerTypesChanged()
    {
    Q_EMIT typesChanged();
    }

void setNodeDirectory( QString in )
    {
    QSettings settings;
    settings.setValue( "synapseNodeDirectory", in );
    nodeTypesChanged();
    }

// return the node directory
QString nodeDirectory()
    {
    //grab the directory from settings, or trigger a dialog to choose one and store it
    QSettings settings;
    if( settings.value( "synapseNodeDirectory" ).toString() == "" )
        {
        QStringList args( QApplication::arguments() );
        if( args.size() < 1 )
            {
            settings.setValue( "synapseNodeDirectory", AFileDialog::getExistingDirectory( QObject::tr( "Choose the node plugin directory" ) ) );
            }
        else
            {
            settings.setValue( "synapseNodeDirectory", QFileInfo( args[0] ).absoluteDir().absolutePath() + "/synapseNodes/" );
            }
        }
    // return the value from settings
    return settings.value( "synapseNodeDirectory" ).toString();
    }

void initiateLibraryNodes( APlugin *plug )
    {
    synapseSDK sdk( plug );
    typedef void (*initFn)( synapseSDK * );

    // foreach file in the nodes directory
    foreach( QFileInfo info, QDir( nodeDirectory() ).entryInfoList(  ) )
        {
        // if its a library
        if( QLibrary::isLibrary( info.absoluteFilePath() ) )
            {
            // if we can resolve the file
            QLibrary lib( info.absoluteFilePath() );
            initFn ptr( (initFn)lib.resolve( "synapseInitFn" ) );
            if( ptr )
                {
                // use the function to initiate the library
                ptr( &sdk );
                }
            }
        }
    nodeTypesChanged();
    }

void registerNode( APlugin *sdk, AEntityType t, AEntityCreatorFn fn, QColor col )
    {
    // add the creator for loading purposes, then register one so we can create one on command
    sdk->addCreator( t, fn );
    codeNodes.insert( t, QPair<AEntityCreatorFn,QColor>(fn,col) );
    }

void unRegisterNode( APlugin *sdk, AEntityType t )
    {
    // remove from the appDB, then remove from the local nodeDB
    sdk->removeCreator( t );
    codeNodes.remove( t );
    }

nodeEntity *createNodeFromXML( QString data, QString type, APlugin * );

QIcon nodeType::icon() const
    {
    // search for the icon in lots of different places...
    QString iconFile;

    // find a png icon in the node directory
    iconFile = nodeDirectory() + "/" + name().toLower() + ".svg";
    if( QFile::exists( iconFile ) )
        {
        return QIcon( iconFile );
        }

    // find a png icon in the node directory
    iconFile = nodeDirectory() + "/" + name().toLower() + ".png";
    if( QFile::exists( iconFile ) )
        {
        return QIcon( iconFile );
        }

    // find a jpg icon in the node directory
    iconFile = nodeDirectory() + "/" + name().toLower() + ".jpg";
    if( QFile::exists( iconFile ) )
        {
        return QIcon( iconFile );
        }

    // return a null icon...
    return QIcon();
    }

nodeEntity *nodeType::createAndInsert( APlugin *app )
    {
    // we want this as one undo step
    app->beginBlockChange( );
    // create a base node
    nodeEntity *ent( create( app ) );

    // rename from "null"
    //ent->setName( ent->array()->getUniquePath( AEntityLocation("/"), name() ).name() );

    if( ent->property("initiated")->toBool() == false )
        {
        // trigger custom node creation commands
        ent->initiate();
        ent->property("initiated") = true;
        }

    // finish change block
    app->endBlockChange();
    //return for further customisation
    return ent;
    }

QStringList groupsAvailable( QFileInfo fileInfo )
    {
    QStringList ret;

    // create a file object around the file requested
    QFile file( fileInfo.absoluteFilePath() );
    if( file.open( QIODevice::ReadOnly ) )
        {
        // weve successfully opened it now. so extract the contents into a DOM object
        QDomDocument doc("synapse");
        doc.setContent( file.readAll() );

        // find the root element
        QDomElement docElem = doc.documentElement();

        // and its first child
        QDomNode nRoot = docElem.firstChild();
        while( !nRoot.isNull() )
            {
            QDomElement node = nRoot.toElement(); // try to convert the node to an element.
            if( !node.isNull() && node.tagName() == "node" ) // if its a node!
                {
                QString group( node.attribute("group") ); // find the group its in "" is ok too...
                if( !ret.contains(group) ) // check its unique
                    {
                    // at this point we add the group name to the list
                    if( group != "" )
                        {
                        ret << group;
                        }
                    else
                        {
                        ret << QObject::tr("Other"); // if "" the we'll call it "Other"
                        }
                    }
                }
            // next node in the root elements list.
            nRoot = nRoot.nextSibling();
            }
        }
    return ret;
    }

QList <nodeType> typesAvailable( QFileInfo fileInfo, QString group="" )
    {
    QList <nodeType> ret;

    // create a file object and try to open it
    QFile file( fileInfo.absoluteFilePath() );
    if( file.open( QIODevice::ReadOnly ) )
        {
        // read all data into a DOM object
        QDomDocument doc("synapse");
        doc.setContent( file.readAll() );

        // find the root element
        QDomElement docElem = doc.documentElement();

        // extract first child
        QDomNode nRoot = docElem.firstChild();
        while( !nRoot.isNull() )
            {
            QDomElement node = nRoot.toElement(); // try to convert the node to an element.
            if( !node.isNull() && node.tagName() == "node" ) // if its a node
                {
                // if its group is equivalant to the input group
                if( group == node.attribute("group") || ( group == QObject::tr("Other") && node.attribute("group") == "" ) )
                    {
                    // parse the colour ( we allow values 0.0 -> 1.0 and 0 -> 255, so differentiate between them...
                    XVariant var( node.attribute( "colour" ) );
                    XList<float> ints( var.toBasicTypeList<float>() );
                    XColour col;

                    if( ints.size() >= 3 )
                        {
                        if( ints[0] > 1.0 || ints[1] > 1.0 || ints[2] > 1.0 )
                            {
                            col = XColour( ints[0]/255.0, ints[1]/255.0, ints[2]/255.0 );
                            }
                        else
                            {
                            col = XColour( ints[0], ints[1], ints[2] );
                            }
                        }
                    else
                        {
                        col = XColour( 0.5, 0.5, 0.5 ); // otherwise set to a default...
                        }
                    // add it to the return list
                    ret << nodeType( fileInfo.absoluteFilePath(), group, node.attribute("name"), node.attribute("icon"), col.toLDRColour() );
                    }
                }
            // next element
            nRoot = nRoot.nextSibling();
            }
        }
    return ret;
    }

QList <QFileInfo> getXmlNodeFiles()
    {
    // for now we return *.xml files in the nodeDirectory, but this may search other directories in future...
    return QDir( nodeDirectory() ).entryInfoList( QStringList( "*.xml" ), QDir::Files );
    }

QList <QFileInfo> getNodeFiles()
    {
    QList <QFileInfo> ret;
    // for now we return *.xml files in the nodeDirectory, but this may search other directories in future...
    foreach( QFileInfo info, QDir( nodeDirectory() ).entryInfoList(  ) )
        {
        if( info.completeSuffix() == "xml" || QLibrary::isLibrary( info.absoluteFilePath() ) )
            {
            ret << info;
            }
        }

    return ret;
    }

QStringList nodeGroups()
    {
    // we extract all groups from all files available and put them into a set ( no duplicates )
    QSet <QString> ret;
    foreach( QFileInfo f, getXmlNodeFiles() )
        {
        ret.unite( groupsAvailable( f ).toSet() );
        }

    // then for each "codeNode"
    foreach( AEntityType t, codeNodes.keys() )
        {
        // format the group, get rid of any /'s
        QString grp( t.group() );
        if( grp[0].toAscii() == '/' )
            {
            grp = grp.mid( 1 );
            }
        grp = grp.replace( "/", "" );
        // and if it isnt there already, insert it.
        if( !ret.contains( grp ) && grp != "" )
            {
            ret << grp;
            }
        }

    // convert the set to a list
    QStringList retList( ret.toList() );
    // and sort it nicely
    retList.sort();
    return retList;
    }

QList <nodeType> listNodeGroup( QString group )
    {
    QList <nodeType> ret;
    // add each xml file to the return
    foreach( QFileInfo f, getXmlNodeFiles() )
        {
        ret << typesAvailable( f, group );
        }

    // for each C++ node...
    foreach( AEntityType t, codeNodes.keys() )
        {
        // format its group
        QString grp( t.group() );
        if( grp[0].toAscii() == '/' )
            {
            grp = grp.mid( 1 );
            }
        grp = grp.replace( "/", "" );

        // format the input group, just in case
        QString otherGrp( group );
        if( otherGrp[0].toAscii() == '/' )
            {
            otherGrp = otherGrp.mid( 1 );
            }
        otherGrp = otherGrp.replace( "/", "" );

        // check theyre equal
        if( grp == otherGrp || ( otherGrp == QObject::tr("Other") && grp == "" ) )
            {
            // and insert if they are.
            nodeType type( t, codeNodes.value( t ).first, codeNodes.value( t ).second );
            if( !ret.contains( type ) )
                {
                ret << type;
                }
            }
        }

    qSort( ret );

    return ret;
    }

nodeType::nodeType( ) : _type( 0 )
    {
    }

bool nodeType::isValid()
    {
    return _name != "";
    }

nodeType::nodeType( AEntityType t, AEntityCreatorFn fn, QColor col ) : _type( new AEntityType( t ) ), _fn( fn ), _colour( col )
    {
    // creating a c++ node type, so the file is obiously ""
    _file = "";

    // group is the types group
    _group = _type->group();

    // but format it nicely
    if( _group[0].toAscii() == '/' )
        {
        _group = _group.mid( 1 );
        }
    _group = _group.replace( "/", "" );

    // and the name is they type name
    _name = _type->name();
    }

nodeType::nodeType( QString group, QString type, QColor col ) : _type( 0 ), _fn( 0 ), _colour( col )
    {
    // create a node from a group and a type, first check the type is not nil
    if( type != "" )
        {
        QList <nodeType> list( listNodeGroup( group ) );

        // then search all known types and make this == that...
        foreach( nodeType r, list )
            {
            if( r.name() == type )
                {
                *this = r;
                return;
                }
            }
        }
    }

nodeType findType( QString group, QString type, bool *found )
    {
    if( found )
        {
        *found = false;
        }
    // search for the node type
    if( type != "" )
        {
        QList <nodeType> list( listNodeGroup( group ) );

        // then search all known types and make this == that...
        foreach( nodeType r, list )
            {
            if( r.name() == type )
                {
                if( found )
                    {
                    *found = true;
                    }
                return r;
                }
            }
        }
    return nodeType();
    }

nodeType::nodeType( QString file, QString group, QString name, QString icon, QColor col ) : _file( file ), _group( group ), _name( name ), _icon( icon ), _type( 0 ), _fn( 0 ), _colour( col )
    {
    // create nodetype from sensible XML data, just format the group, and were done
    if( _group[0].toAscii() == '/' )
        {
        _group = _group.mid( 1 );
        }
    _group = _group.replace( "/", "" );
    }

nodeType::nodeType( QVariant var ) : _file( var.toList()[0].toString() ), _group( var.toList()[1].toString() ),
        _name( var.toList()[2].toString() ), _icon( var.toList()[3].toString() ),
        _type( new AEntityType( var.toList()[4].toString() ) ),
        _fn( (AEntityCreatorFn)var.toList()[5].toULongLong() ),
        _colour( var.toList()[6].value<QColor>() )
    {
    // re construct a serialised nodeType, but format the group again to be sure
    if( _group[0].toAscii() == '/' )
        {
        _group = _group.mid( 1 );
        }
    _group = _group.replace( "/", "" );
    }

nodeType::nodeType( const nodeType &in ) : _file( in._file ), _group( in._group ), _name( in._name ), _icon( in._icon ), _type( 0 ), _fn( in._fn ), _colour( in._colour )
    {
    if( in._type )
        {
        _type = new AEntityType( *in._type );
        }
    }

nodeType::~nodeType()
    {
    // delete the type if it exists
    if( _type )
        {
        delete _type;
        _type = 0;
        }
    }

QColor nodeType::colour() const
    {
    return _colour;
    }

QString nodeType::group() const
    {
    return _group;
    }

QString nodeType::name() const
    {
    return _name;
    }

QString nodeType::file() const
    {
    return _file;
    }

nodeType::operator QVariant()
    {
    // compress to a variant...
    QVariantList var;

    // stream string data
    var << _file << _group << _name << _icon;

    // if the _type exists, stream it, otherwise stream ""
    if( _type )
        {
        var << *_type;
        }
    else
        {
        var << "";
        }

    // and stream the pointer to the fn ( this is ok, because we never store these variants... but is still a bit sticky
    var << (qulonglong)_fn;

    var << _colour;

    return var;
    }

bool nodeType::operator<( const nodeType &in ) const
    {
    // test group and name for less than
    return group() <= in.group() && name() < in.name();
    }

bool nodeType::operator==( const nodeType &in ) const
    {
    // test group and name for equality
    return group() == in.group() && name() == in.name();
    }

nodeType &nodeType::operator=( const nodeType &in )
    {
    // assign string values ( after formatting )
    _file = in._file ;
    _group = in._group;

    if( _group[0].toAscii() == '/' )
        {
        _group = _group.mid( 1 );
        }
    _group = _group.replace( "/", "" );

    _name = in._name;
    _icon = in._icon;

    // re assgin type ( after clearing it if required )
    if( _type )
        {
        delete _type;
        }
    if( in._type )
        {
        _type = new AEntityType( *in._type );
        }
    else
        {
        _type = 0;
        }

    // and copy the function pointer
    _fn = in._fn;

    _colour = in._colour;

    return *this;
    }

nodeEntity *nodeType::create( APlugin *app )
    {
    // either create from xml, if the file exists
    nodeEntity *ret = 0;
    QFile file( _file );
    if( file.open( QIODevice::ReadOnly ) )
        {
        ret = createNodeFromXML( file.readAll(), name(), app );
        }

    // otherwiser its probably a C++ node type and we should try to create it...
    if( ret == 0 && _fn )
        {
        ret = (*_fn)()->castTo<nodeEntity*>();
        app->addEntity( ret );
        }

    return ret;
    }

void deleteNode( nodeType type )
    {
    if( type.file() != "" )
        {
        QFile file( type.file() );
        if( file.exists() && !file.open( QFile::ReadOnly ) )
            {
            AWarning << QObject::tr( "Couldn't open file %1 for reading" ).arg( type.file() );
            }

        QDomDocument doc("synapse");
        doc.setContent( file.readAll() );
        file.close();

        // root element
        QDomElement docElem = doc.documentElement();

        if( docElem.tagName() == "synapse" )
            {
            QDomNode nRoot = docElem.firstChild();
            while( !nRoot.isNull() )
                {
                QDomElement node = nRoot.toElement(); // try to convert the node to an element.
                if( !node.isNull() && node.tagName() == "node" && node.attribute("name") == type.name() ) // if its the correct type
                    {
                    docElem.removeChild( nRoot );
                    }
                // find the next tag in the root tag
                nRoot = nRoot.nextSibling();
                }
            }

        if( !file.open( QFile::WriteOnly ) )
            {
            AWarning << QObject::tr( "Couldn't open file %1 for writing" ).arg( type.file() );
            }
        file.write( doc.toString().toAscii() );
        file.close();

        nodeTypesChanged();
        }
    else
        {
        AWarning << QObject::tr( "Couldn't find the file containing node %1 to delete." ).arg( type.name() );
        }
    }

void saveNode( APlugin *app, nodeEntity *ent )
    {
    nodeType type( findType( ent->property("nodeEntity/group")->toString(), ent->property("nodeEntity/type")->toString() ) );

    // find the filename, or create one
    QString filename;
    if( type.file() != "" )
        {
        filename = type.file();
        }
    else if( ent->property("nodeEntity/group")->toString() != "" )
        {
        filename = nodeDirectory() + "/" + ent->property("nodeEntity/group")->toString() + ".xml";
        }
    else
        {
        filename = nodeDirectory() + "/Other.xml";
        }

    QFile file( filename );
    if( file.exists() && !file.open( QFile::ReadOnly ) )
        {
        AWarning << QObject::tr( "Couldn't open file %1 for reading" ).arg( filename );
        }

    // create the document
    QDomDocument doc("synapse");
    doc.setContent( file.readAll() );
    file.close();

    // root element
    QDomElement docElem = doc.documentElement();

    // if its a synapse node document
    if( docElem.tagName() == "synapse" )
        {
        QDomNode nRoot = docElem.firstChild();
        while( !nRoot.isNull() )
            {
            QDomElement node = nRoot.toElement(); // try to convert the node to an element.
            if( !node.isNull() && node.tagName() == "node" && node.attribute("name") == type.name() ) // if its the correct type
                {
                // remove the old node
                docElem.removeChild( nRoot );
                }
            // find the next tag in the root tag
            nRoot = nRoot.nextSibling();
            }
        }
    else
        {
        doc = QDomDocument("syanpse");
        docElem = doc.appendChild( doc.createElement("synapse") ).toElement();
        }

    // create a root element
    QDomElement root( doc.createElement("node") );

    // add the name attribute
    root.setAttribute( "name", ent->property("nodeEntity/type")->toString() );
    // add the group attribute
    if( ent->property("nodeEntity/group")->toString() != "" )
        {
        root.setAttribute( "group", ent->property("nodeEntity/group")->toString() );
        }
    // add the colour attribute
    root.setAttribute( "colour", ent->property("nodeEntity/colour")->toString() );

    if( ent->type( ) == groupNode::identifier() )
        {
        // add the colour attribute
        root.setAttribute( "nodeType", "group" );

        AEntitySearch search;
        search << ent;
        QByteArray arr( app->copy( search ) );

        // add the source text
        QDomText sourceText = doc.createTextNode( QString::fromUtf8( arr.toHex() ) );
        root.appendChild( sourceText );
        }
    else
        {
        // add the source text
        QDomText sourceText = doc.createTextNode( ent->property("nodeEntity/source")->toString() );
        QDomElement source( doc.createElement("source") );
        source.appendChild( sourceText );
        root.appendChild( source );

        // add the short help
        QDomText shortHelpText = doc.createTextNode( ent->property("shortHelp")->toString() );
        QDomElement shortHelpTag( doc.createElement("shortHelp") );
        shortHelpTag.appendChild( shortHelpText );
        root.appendChild( shortHelpTag );

        // add the long help
        QDomText longHelpText = doc.createTextNode( ent->property("longHelp")->toString() );
        QDomElement longHelpTag( doc.createElement("longHelp") );
        longHelpTag.appendChild( longHelpText );
        root.appendChild( longHelpTag );

        // add each property if its needed
        foreach( AProperty *prop, ent->properties() )
            {
            // if its an input, add it with an input tag
            if( prop->attributes().contains( "Linkable" ) && prop->name() != "output" )
                {
                QDomElement inputTag( doc.createElement("input") );
                QDomText inputName = doc.createTextNode( prop->name() );
                inputTag.appendChild( inputName );
                root.appendChild( inputTag );
                }
            else if( prop->attributes().contains("Attribute") )
                {
                // add attributes with a value and type, and with an attribute tag
                QDomElement attrTag( doc.createElement("attribute") );
                QDomText attrName = doc.createTextNode( prop->name() );
                attrTag.appendChild( attrName );
                root.appendChild( attrTag );
                attrTag.setAttribute( "value", prop->value().toString() );

                // this could be less hard coded....
                switch( prop->type() )
                    {
                    case calcPropertyInfo::Colour:
                        attrTag.setAttribute( "type", "colour" );
                        break;
                    case propertyInfo::Matrix3x3:
                        attrTag.setAttribute( "type", "matrix3x3" );
                        break;
                    case propertyInfo::Component2D:
                        attrTag.setAttribute( "type", "position2D" );
                        break;
                    case propertyInfo::Component3D:
                        attrTag.setAttribute( "type", "position3D" );
                        break;
                    case propertyInfo::Component4D:
                        attrTag.setAttribute( "type", "position4D" );
                        break;
                    case propertyInfo::Double:
                        attrTag.setAttribute( "type", "double" );
                        break;
                    default: break;
                    }
                }
            }
        }
    // append the root element to the docElement
    docElem.appendChild( root );

    // write the data
    if( !file.open( QFile::WriteOnly ) )
        {
        AWarning << QObject::tr( "Couldn't open file %1 for writing" ).arg( filename );
        }
    file.write( doc.toString().toAscii() );
    file.close();

    // signal a type change
    nodeTypesChanged();
    }

nodeEntity *createNodeFromXML( QString data, QString type, APlugin *app )
    {
    // create a DOM
    QDomDocument doc("synapse");
    doc.setContent( data );

    // root element
    QDomElement docElem = doc.documentElement();

    QDomNode nRoot = docElem.firstChild();
    while( !nRoot.isNull() )
        {
        QDomElement node = nRoot.toElement(); // try to convert the node to an element.
        if( !node.isNull() && node.tagName() == "node" && node.attribute("name") == type ) // if its the correct type
            {
            if( node.attribute("nodeType") == "group" )
                {
                QByteArray contents( QByteArray::fromHex( node.text().toUtf8() ) );
                AEntitySearch search( app->search( ).findDirectChildren( app->workingPath() ) );

                app->paste( contents );
                AEntitySearch ret( app->search().findDirectChildren( app->workingPath() ) );
                ret.remove( search );

                nodeEntity *ent( ret[0].castTo<nodeEntity*>() );
                ent->setName( ent->array()->getUniquePath( AEntityLocation("/"), ent->property("nodeEntity/type")->toString() ).name() );

                if( ent->drawNode() )
                    {
                    ent->drawNode()->centre();
                    }
                ent->property("initiated") = false;
                ent->property("propertiesState") = 0;
                ent->disconnect();
                ent->resetProperties();

                return ent;
                }
            else
                {
                nodeEntity *ret( nodeEntity::create()->castTo<nodeEntity*>() );
                ret->property("editable") = true;
                ret->property("nodeEntity/type") = node.attribute( "name" ); // set up the type and group
                ret->property("nodeEntity/group") = node.attribute( "group" );

                // parse the colour ( we allow values 0.0 -> 1.0 and 0 -> 255, so differentiate between them...
                XVariant var( node.attribute( "colour" ) );
                XList<float> ints( var.toBasicTypeList<float>() );

                if( ints.size() >= 3 )
                    {
                    if( ints[0] > 1.0 || ints[1] > 1.0 || ints[2] > 1.0 )
                        {
                        ret->property("nodeEntity/colour") = XColour( ints[0]/255.0, ints[1]/255.0, ints[2]/255.0 );
                        }
                    else
                        {
                        ret->property("nodeEntity/colour") = XColour( ints[0], ints[1], ints[2] );
                        }
                    }
                else
                    {
                    ret->property("nodeEntity/colour") = XColour( 0.5, 0.5, 0.5 ); // otherwise set to a default...
                    }

                // find first child
                QDomNode cRoot = node.firstChild();
                while( !cRoot.isNull() )
                    {
                    QDomElement nodeTag = cRoot.toElement(); // try to convert the node to an element.
                    if( !nodeTag.isNull() )
                        {
                        QString name( nodeTag.tagName() );
                        if( name == "input" ) // if its an input tag, add an input
                            {
                            ret->addInput( nodeTag.text(), nodeTag.text(), nodeTag.attribute("default").toInt() );
                            }
                        else if( name == "attribute" ) // or an attribute, add an attribute
                            {
                            // this list should be extended at some point...
                            int val = propertyInfo::Double;
                            QString type( nodeTag.attribute("type") );
                            if( type == "colour" )
                                {
                                val = calcPropertyInfo::Colour;
                                }
                            else if( type == "position2D" )
                                {
                                val = propertyInfo::Component2D;
                                }
                            else if( type == "position3D" )
                                {
                                val = propertyInfo::Component3D;
                                }
                            else if( type == "position4D" )
                                {
                                val = propertyInfo::Component4D;
                                }
                            else if( type == "matrix3x3" )
                                {
                                val = propertyInfo::Matrix3x3;
                                }
                            else
                                {
                                val = propertyInfo::Double;
                                }
                            ret->addAttribute(  nodeTag.text(), nodeTag.attribute("value"), val );
                            }
                        else if( name == "source" )
                            {
                            ret->property( "nodeEntity/source" ) = nodeTag.text(); // and extract the source from the xml
                            }
                        else if( name == "shortHelp" )
                            {
                            ret->property( "shortHelp" ) = nodeTag.text(); // and extract the short help
                            }
                        else if( name == "longHelp" )
                            {
                            ret->property( "longHelp" ) = nodeTag.text(); // and extract the long help
                            }
                        }
                    // find the nex tag in the node tag
                    cRoot = cRoot.nextSibling();
                    }

                app->addEntity( ret );
                ret->setName( ret->array()->getUniquePath( AEntityLocation("/"), ret->property("nodeEntity/type")->toString() ).name() );
                return ret;
                }
            }
        // find the next tag in the root tag
        nRoot = nRoot.nextSibling();
        }
    return 0;
    }

}
