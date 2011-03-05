#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include "QStandardItemModel"
#include "AEntitySearch"

class entityModel : public QStandardItemModel
    {
    Q_OBJECT
    XROProperty( APlugin *, app )
public:
    entityModel( APlugin *, QObject *parent );

public slots:
    void refresh( );

private slots:
    void watch(AEntity *);
    void entityChange(APropertyDevice *, AProperty *);
    };

#endif // ENTITYMODEL_H
