#include "../SDK/APropertyManager"
#include "../SDK/AEntity"
#include "QMultiMap"
#include "QLabel"
#include "QGroupBox"
#include "QVBoxLayout"
#include "QFormLayout"
#include "QScrollArea"
#include "../SDK/ALog"
#include "APrivatePropertyWidgetFactory.h"

/**  \cond INTERNAL
  \class AScrollArea
  \brief Internal class to property manager that uses the size hint of the widget rather than a set size.
  */
class AScrollArea : public QScrollArea
    {
public:
    AScrollArea( QWidget *parent=0 ) : QScrollArea( parent ) { }
    virtual QSize sizeHint( ) const { return widget()->sizeHint() + QSize( 0, 2 ); }
    };
/** \endcond */

/** \class APropertyManager
  \brief Allows creation of UI elements related to properties.
  */

/** Create a property manager
  */
APropertyManager::APropertyManager() : _mode( Compact )
    {
    }

/** Add an entity to the manager.
  */
void APropertyManager::addEntity( AEntity *in )
    {
    // only unique values
    if( !_devices.contains( in ) && in )
        {
        _devices << in;
        }
    }

/** Add a property to the manager.
  */
void APropertyManager::addProperty( AProperty &in )
    {
    // unique property
    if( !_properties.contains( &in ) )
        {
        _properties << &in;
        }
    }

/** Get a widget for the property \p in, with no label. Useful for inserting properties into your own dialogs and GUI elements
  */
QWidget *APropertyManager::createWidget( AProperty &in )
    {
    APrivatePropertyWidgetFactory factory;
    return factory.widgetFor( &in );
    }

/** \overload
    Get a widget for the property \p in, with no label. Useful for inserting properties into your own dialogs and GUI elements. \p forceType allows you to force the type of the widget to another GUI type (for example propertyInfo::Bool will alwyas return a bool control)
  */
QWidget *APropertyManager::createWidget( AProperty &in, int forceType )
    {
    APrivatePropertyWidgetFactory factory;
    return factory.widgetFor( &in, forceType );
    }

/** Create a widget for the entity \p in.
  */
QWidget *APropertyManager::createWidget( AEntity *in )
    {
    APropertyManager m;
    m.addEntity( in );
    return m.createWidget();
    }

/** Set the display mode to \p m.
  \todo Implement these modes.
  */
void APropertyManager::setMode( propertyMode m )
    {
    _mode = m;
    }

typedef QMap <QString, AProperty *> propertyList;

/** Create a widget for the properties and entities contained in the manager
  */
QWidget *APropertyManager::createWidget( ) const
    {
    APrivatePropertyWidgetFactory factory;
    // only option at the moment
    if( _mode == Compact )
        {
        // a list with all the properties in it
        QMap <QString, propertyList> finalList;

        foreach( AEntity *dev, _devices )
            {
            propertyList temp;
            foreach( AProperty *prop, dev->properties() )
                {
                // no hidden properties
                if( !prop->hidden() )
                    {
                    // insert into the map, with name as the key, so theyre sorted
                    temp.insert( prop->displayName(), prop );
                    }
                }
            //dont inert empty entities
            if( !temp.isEmpty() )
                {
                finalList.insert( dev->name(), temp );
                }
            }
        // for each property
        foreach( AProperty *data, _properties )
            {
            // find the property name
            QString key = data->name();

            // and its parent
            AEntity *parent( data->parent()->castTo<AEntity*>() );
            if( parent )
                {
                // if we found the parent, try to find its name so we can group properties
                key = parent->name();
                }

            // insert the property into a list
            propertyList temp;
            temp.insert( data->displayName(), data );

            // if the list already contains a key the same as ours
            if( finalList.contains( key ) )
                {
                // append
                propertyList preList( finalList.value( key ) );
                preList = preList.unite( temp );
                finalList[key] = preList;
                }
            else
                {
                // otherwise add it to the lsit
                finalList.insert( key, temp );
                }
            }

        //new widget for the properties
        QWidget *tree = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout( tree );

        // for each entity name the map
        foreach( QString str, finalList.keys() )
            {
            propertyList list( finalList.value( str  ));
            QWidget *currentParent = tree;
            // dont add titles for just 1 property
            if( finalList.size() > 1 )
                {
                // add a group box for it
                currentParent = new QGroupBox( str, tree );
                layout->addWidget( currentParent );
                }
            // create a layout for the properties
            QFormLayout *propertiesLayout( new QFormLayout( currentParent ) );

            // if we have a group box, then add the new layout to that, otherwise it will be automatically
            // be in the parent layout
            if( finalList.size() <= 1 )
                {
                layout->addLayout( propertiesLayout );
                }

            // align label right, its prettier
            propertiesLayout->setLabelAlignment( Qt::AlignRight );

            QFontMetrics fM( tree->font() );

            // for each property
            foreach( AProperty *prop, list )
                {
                // create a ui widget
                QWidget *widg( factory.widgetFor( prop ) );
                widg->setParent( currentParent );

                // and a nice trimmed ( labe... ) label
                QLabel *label( new QLabel( fM.elidedText( prop->displayName(), Qt::ElideRight, 100 ), currentParent ) );
                label->setToolTip( prop->displayName() );

                // add the row
                propertiesLayout->addRow( label, widg );
                }
            }

        // add a stretch
        layout->addStretch();

        // put in a scroll area
        AScrollArea *scrollArea( new AScrollArea );
        scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        scrollArea->setWidget( tree );
        scrollArea->setWidgetResizable( TRUE );

        return scrollArea;
        }

    qDebug() << "Incorrect mode specified!";
    return 0;
    }
