#ifndef ACORE_H
#define ACORE_H

#include "aglobal.h"
#include "QString"
#include "XList"
#include "XHash"
#include "XObject"

class ACorePrivate;
class AAbstractPlugin;

class ALTER_EXPORT ACore : public QObject
    {
    Q_OBJECT

public:
    ACore( int argv, char **argc );
    ~ACore();

    void quit();

    static QString rootPath();

    bool isLoaded( const QString &name ) const;
    bool load( const QString &name );
    void unload( const QString &name );
    QStringList loadable() const;

    QStringList directories() const;
    void addDirectory(const QString &);
    void removeDirectory(const QString &);

    QObject *plugin(const QString &pluginID);

    bool isDebugUIEnabled() const;

    int execute();
private:
    ACorePrivate *_priv;
    friend class APluginBase;
    };

#endif // ACORE_H
