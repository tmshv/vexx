#ifndef FILETYPEBASE_H
#define FILETYPEBASE_H

class AAbstractFileType;

#include "../SDK/AGlobal"
#include "../SDK/AEntitySearch"
#include "../SDK/AProperty"

class ALTER_EXPORT AAbstractFileType
    {
    XProperty( QString, name, setName );
    XRefProperty( QList <QString>, extensions );

public:
    AAbstractFileType( QString name );
    virtual ~AAbstractFileType();
    QString getFilterString() const;
    QString getDefaultSuffix() const;
    virtual QByteArray createFileContents( ) const = 0;
    virtual void parseFileContents( const QByteArray & ) = 0;
    void clear();

    struct property { AVariant value; QHash <QString, AVariant> attributes; };
    struct entity { QString type; QHash <QString, property> properties; };

    QHash <QString, entity> &entities();
    const QHash <QString, entity> &entities() const;

private:
    QHash <QString, entity> _entities;
    };

#endif
