#ifndef IIMAGELIBRARY_H
#define IIMAGELIBRARY_H

#include "QString"
#include "QList"
#include "iimage.h"
#include "idirectorycontrol.h"

class IImageLibrary
    {
public:
    IImageLibrary( QString );
    ~IImageLibrary( );

    void index();
    QList <IImage> files();

private:
    QString _base;
    QList <IDirectoryControl *> _dirs;
    };

#endif // IIMAGELIBRARY_H
