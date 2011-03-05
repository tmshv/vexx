#include "AEntityModel.h"
#include "AEntityArray"

enum type { Entity=0, Property=1, Attribute=2 };

union indexData
    {
    quint32 longInt;
    struct
        {
        quint32 type : 2;
        quint32 entity : 16;
        quint32 property : 7;
        quint32 attribute : 7;
        };
    };

AEntityModel::AEntityModel( AEntityArray *array, QObject *parent ) : QAbstractItemModel( parent ), _array( array )
    {
    }

int AEntityModel::rowCount( const QModelIndex &parent ) const
    {
    indexData data; data.longInt = parent.internalId();
    if( !parent.isValid() )
        {
        return _array->size();
        }
    else if( data.type == Entity )
        {
        AEntity *ent( _array->entities()[data.entity] );
        return ent->properties().size();
        }
    else if( data.type == Property )
        {
        AEntity *ent( _array->entities()[data.entity] );

        AProperty *prop( 0 );

        int x=0;
        foreach( AProperty *tmpProp, ent->properties( ) )
            {
            if( x == data.property )
                {
                prop = tmpProp;
                break;
                }
            x++;
            }
        return prop->attributes().size();
        }

    return 0;
    }

QModelIndex AEntityModel::index( int row, int column, const QModelIndex &parent ) const
    {
    indexData data; data.longInt = parent.internalId();
    if( !parent.isValid() && row >= 0 )
        {
        indexData ret;
        ret.entity = row;
        ret.type = Entity;
        return createIndex( row, column, ret.longInt );
        }
    else if( data.type == Entity )
        {
        indexData ret;
        ret.entity = parent.row();
        ret.property = row;
        ret.type = Property;
        return createIndex( row, column, ret.longInt );
        }
    else if( data.type == Property )
        {
        indexData ret;
        ret.entity = data.entity;
        ret.property = row;
        ret.type = Attribute;
        return createIndex( row, column, ret.longInt );
        }
    return QModelIndex();
    }

QModelIndex AEntityModel::parent( const QModelIndex &child ) const
    {
    indexData data; data.longInt = child.internalId();
    if( data.type == Property )
        {
        data.type = Entity;
        data.property = 0;
        return createIndex( 0, 0, data.longInt );
        }
    else if( data.type == Attribute )
        {
        data.type = Property;
        data.property = 0;
        data.attribute = 0;
        return createIndex( 0, 0, data.longInt );
        }
    return QModelIndex();
    }

int AEntityModel::columnCount( const QModelIndex & ) const
    {
    return 2;
    }

QVariant AEntityModel::data( const QModelIndex &index, int role ) const
    {
    if( role == Qt::DisplayRole )
        {
        indexData data; data.longInt = index.internalId();
        if( data.type == Entity )
            {
            AEntity *ent( _array->entities()[data.entity] );

            if( index.column() == 0 )
                {
                return ent->name();
                }
            else if( index.column() == 1 )
                {
                return ent->type().group() + "/" + ent->type().name();
                }
            }
        else if( data.type == Property )
            {
            AEntity *ent( _array->entities()[data.entity] );

            AProperty *prop( 0 );

            int x=0;
            foreach( AProperty *tmpProp, ent->properties( ) )
                {
                if( x == data.property )
                    {
                    prop = tmpProp;
                    break;
                    }
                x++;
                }

            if( index.column() == 0 )
                {
                return prop->name();
                }
            else if( index.column() == 1 )
                {
                return prop->value().toPortable( XVariant::Ascii );
                }
            }
        }

    return QVariant();
    }
