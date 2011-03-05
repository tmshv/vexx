#ifndef APPSCRIPT_H
#define APPSCRIPT_H

#include "alterSDK.h"
#include <QObject>
#include "QStringList"

class QScriptEngine;

class appScript : public QObject
    {
    Q_OBJECT
    jPtrProperty( alterSDKBase, app )
public:
    appScript( QObject *parent=0 );
    void added();
    void execute( QString );

public slots:
    void log( QString );
    QString addEntity( QString );
    void removeEntity( QString );
    QStringList entities( );

    void addProperty( QString, QString, QString );
    void removeProperty( QString, QString );
    QStringList properties( QString );
    QVariant getProperty( QString, QString );
    void setProperty( QString, QString, QString );

    QVariant attribute( QString, QString, QString );
    void setAttribute( QString, QString, QString, QString );
    QStringList attributes( QString, QString );

private:
    QScriptEngine *_engine;
    };

#endif // APPSCRIPT_H
