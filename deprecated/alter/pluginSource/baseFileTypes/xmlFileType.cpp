#include "xmlFileType.h"
#include "QDomDocument"

xmlFileType::xmlFileType() : AAbstractFileType( "XML File" )
    {
    extensions() << "xml" << "jml";
    }

QByteArray xmlFileType::createFileContents() const
    {
    QDomDocument doc("alterDocument");
    QDomElement root = doc.createElement("alterDocument");
    doc.appendChild(root);

    QDomElement entitiesTag = doc.createElement("entities");
    root.appendChild(entitiesTag);

    foreach( QString name, entities().keys() )
        {
        entity ent( entities().value( name ) );

        QDomElement entTag = doc.createElement("entity");
        entitiesTag.appendChild(entTag);

        entTag.setAttribute( "name", name );
        entTag.setAttribute( "type", ent.type );

        foreach( QString propertyName, ent.properties.keys() )
            {
            property propertyValue( ent.properties.value( propertyName ) );

            QDomElement propertyTag = doc.createElement("property");
            entTag.appendChild(propertyTag);
            propertyTag.setAttribute( "name", propertyName );

            foreach( QString attributeName, propertyValue.attributes.keys() )
                {
                AVariant attributeValue( propertyValue.attributes.value( attributeName ) );
                propertyTag.setAttribute( attributeName, QString::fromUtf8( attributeValue.toPortable() ) );
                }

            QDomText propertyText = doc.createTextNode( QString::fromUtf8( propertyValue.value.toPortable() ) );
            propertyTag.appendChild( propertyText );
            }
        }

    return doc.toString().toUtf8();
    }

void xmlFileType::parseFileContents( const QByteArray &in )
    {
    QDomDocument doc("alterDocument");
    doc.setContent( QString::fromUtf8( in ) );

    QDomElement docElement( doc.documentElement() );

    QDomElement entitiesElement( docElement.firstChild().toElement() );

    if( !entitiesElement.isNull() && entitiesElement.tagName() == "entities" )
        {
        QDomNode entityNode = entitiesElement.firstChild();
        while( !entityNode.isNull() )
            {
            QDomElement entityElement = entityNode.toElement();
            if( !entityElement.isNull() && entityElement.tagName() == "entity" )
                {
                entity tempEntity;

                QDomNode propertyNode = entityElement.firstChild();
                while( !propertyNode.isNull() )
                    {
                    QDomElement propertyElement = propertyNode.toElement();
                    if( !propertyElement.isNull() && propertyElement.tagName() == "property" )
                        {
                        property propData;

                        QDomNamedNodeMap attributes( propertyElement.attributes() );
                        for( int x=0; x<attributes.size(); x++ )
                            {
                            QDomNode attributeNode( attributes.item( x ) );
                            QDomAttr attribute( attributeNode.toAttr() );
                            if( !attribute.isNull() && attribute.name() != "name" )
                                {
                                propData.attributes[attribute.name()].fromPortable( attribute.value().toUtf8() );
                                }
                            }

                        propData.value.fromPortable( propertyElement.text().toUtf8() );
                        tempEntity.properties.insert( propertyElement.attribute("name"), propData );
                        }
                    propertyNode = propertyNode.nextSibling();
                    }

                tempEntity.type = entityElement.attribute("type");
                entities().insert( entityElement.attribute("name"), tempEntity );
                }
            entityNode = entityNode.nextSibling();
            }
        }
    }
