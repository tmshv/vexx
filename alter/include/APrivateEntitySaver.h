#ifndef PROPERTYENTITYSAVER_H
#define PROPERTYENTITYSAVER_H

/** \cond INTERNAL
  */

class APrivateEntitySaver;

#include "../SDK/AGlobal"
#include "../SDK/AAbstractFileType"
#include "../SDK/AEntityArray"
#include "APrivateEntityCreators.h"

struct APrivateEntityCreatorFn;

class ALTER_EXPORT APrivateEntitySaver
    {
public:
    virtual ~APrivateEntitySaver();
    void addFileType( QString, AAbstractFileType * );
    void removeFileType( QString );

    QString defaultFileSuffix() const;
    QString fileFilterList() const;
    AAbstractFileType *findFileTypeFromExtension( QString ) const;
    AAbstractFileType *findFileTypeFromFilterString( QString ) const;

    bool isValid() const;

    void insert( QByteArray, AEntityArray &, const propertyEntityCreators &, AAbstractFileType *fileType = 0 );
    QByteArray extract( const AEntitySearch &, AAbstractFileType *fileType = 0 ) const;

protected:
    bool fillArrayFromFile( AEntityArray &, const propertyEntityCreators &, QString, QString );
    bool fillFileFromSearch( const AEntitySearch &, QString, QString ) const;

private:
    bool fillArrayFromFileEngine( AEntityArray &, const propertyEntityCreators &, const AAbstractFileType *, bool insert );
    void fillFileEngineFromSearch( const AEntitySearch &, AAbstractFileType *, bool extract ) const;

    QHash <QString, AAbstractFileType *> _fileTypes;
    };

/** \endcond
  */

#endif
