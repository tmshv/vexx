#include "entityModel.h"

entityModel::entityModel( APlugin *app, QObject *parent ) : QStandardItemModel( parent ), _app( app )
    {
    connect( _app, SIGNAL(entityAdded(AEntity*)), this, SLOT(refresh()) );
    connect( _app, SIGNAL(entityAdded(AEntity*)), this, SLOT(watch(AEntity*)) );
    connect( _app, SIGNAL(entityRemoved(AEntity*)), this, SLOT(refresh()) );
    }

void entityModel::refresh( )
    {
    AEntitySearch search( app()->search( ) );

    clear();
    foreach( AEntity *ent, search )
        {
        QList <QStandardItem *> items;

        QStandardItem *entItem( new QStandardItem( ent->location().path() ) );

        foreach( AProperty *prop, ent->properties( ) )
            {
            QList <QStandardItem *> property;

            QStandardItem *propItem( new QStandardItem( prop->name() ) );

            foreach( QString attrName, prop->attributes().keys() )
                {
                QList <QStandardItem *> property;
                property << new QStandardItem( attrName ) << new QStandardItem( QString::fromUtf8( prop->attributes()[attrName].toPortable( XVariant::Ascii ) ) );
                propItem->appendRow( property );
                }

            property << propItem << new QStandardItem( QString::fromUtf8( prop->value().toPortable( XVariant::Ascii ) ) );
            entItem->appendRow( property );
            }

        items << entItem << new QStandardItem( ent->type().group() + ent->type().name() );
        appendRow( items );
        }
    }

void entityModel::watch( AEntity *ent )
    {
    connect( ent, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChange(APropertyDevice*,AProperty*)) );
    }

void entityModel::entityChange( APropertyDevice *, AProperty *prop )
    {
    if( !prop || prop->name() == "entity/name" || prop->name() == "entity/directory" )
        {
        refresh();
        }
    }
