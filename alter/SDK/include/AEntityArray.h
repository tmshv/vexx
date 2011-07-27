#ifndef PROPERTYENTITYARRAY_H
#define PROPERTYENTITYARRAY_H

#include "AGlobal.h"
#include "AEntity.h"
#include "AEntitySearch.h"
#include "APlugin.h"

typedef QList <AEntity *> entityList;

class ALTER_EXPORT AEntityArray : public QObject
    {
    Q_OBJECT
public:
    AEntityArray( );
    ~AEntityArray( );
    void add( AEntity & );
    void remove( AEntity & );
    void clear();
    unsigned int size() const;
    AEntity &operator[]( unsigned int );
    const AEntity &operator[]( unsigned int ) const;

    AEntityLocation workingPath() const;
    void setWorkingPath( AEntityLocation );

    AEntitySearch search();
    AEntitySearchConst search() const;

    bool pathIsValid( AEntityLocation );
    bool pathIsUnique( AEntityLocation );
    AEntityLocation getUniquePath( AEntityLocation, QString );

    const entityList &entities();

Q_SIGNALS:
    void workingPathChanged();
    void entityAdded( AEntity * );
    void entityRemoved( AEntity * );
    /** \cond INTERNAL */
    void onCommand( APrivateCommand** );
    /** \endcond */

private:
    entityList _entities;

    /** \cond INTERNAL */
    friend class APrivateArrayCommand;
    /** \endcond */
    };

#endif
