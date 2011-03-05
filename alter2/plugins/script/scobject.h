#ifndef SCOBJECT_H
#define SCOBJECT_H

#include "scglobal.h"
#include "QObject"

class SCRIPT_EXPORT ScObject : public QObject
    {
    Q_OBJECT
public:
    ScObject( QString name = "" );
    };

#endif // SCOBJECT_H
