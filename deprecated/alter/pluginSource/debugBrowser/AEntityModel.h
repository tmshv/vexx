#ifndef AENTITYMODEL_H
#define AENTITYMODEL_H

#include "QAbstractItemModel"

class AEntityArray;

class AEntityModel : public QAbstractItemModel
    {
    Q_OBJECT
public:
    AEntityModel( AEntityArray *, QObject *parent = 0 );

    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex &child ) const;
    virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

private:
    AEntityArray *_array;
    };

#endif // AENTITYMODEL_H
