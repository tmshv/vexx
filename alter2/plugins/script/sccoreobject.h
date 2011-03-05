#ifndef SCCOREOBJECT_H
#define SCCOREOBJECT_H

#include "scobject.h"
#include "XObject"

class ScPlugin;

class ScCoreObject : public ScObject, public XObject
    {
    Q_OBJECT
    X_OBJECT( ScCoreObject, XObject, 105 )
public:
    ScCoreObject( ScPlugin *plugin );

    void triggerApplicationStarted();
    void triggerApplicationEnded();
    
signals:
    void applicationStarted();
    void applicationEnded();

public slots:
    void quit();
    void include( QString );

private:
    ScPlugin *_plugin;
    };

#endif // SCCOREOBJECT_H
