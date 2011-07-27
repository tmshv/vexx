#ifndef BASETOOLBAR_H
#define BASETOOLBAR_H

#include "QToolBar"
#include "APlugin.h"


class ALTER_EXPORT AAbstractToolBar : public QToolBar
    {
    XProperty( APlugin *, app, setApp );

public:
    AAbstractToolBar();
    virtual void added();
    };

#endif // BASETOOLBAR_H
