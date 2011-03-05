#ifndef IDIRECTORYCONTROL_H
#define IDIRECTORYCONTROL_H

#include "QString"
#include "QDomDocument"
#include "QList"
#include "iimage.h"

class IDirectoryControl
    {
public:
    IDirectoryControl( QString directory );
    ~IDirectoryControl();

    QList <IImage> files();

private:
    Q_DISABLE_COPY( IDirectoryControl );
    QDomDocument _doc;
    QString _directory;
    QList <IImage> _images;
    };

#endif // IDIRECTORYCONTROL_H
