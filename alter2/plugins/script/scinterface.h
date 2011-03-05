#ifndef SCINTERFACE_H
#define SCINTERFACE_H

#include "scglobal.h"
#include "XObject"

class ScPlugin;
class ScObject;

class SCRIPT_EXPORT ScInterface : public XObject
    {
    X_OBJECT( ScInterface, XObject, 104 )
public:
    enum { PluginType = 103 };
    ScInterface( ScPlugin * );


    void registerScriptObject( ScObject * );
    void registerScriptGlobal( ScObject * );
    void execute( QString );

private:
    ScPlugin *_plugin;
    };

#endif // SCINTERFACE_H
