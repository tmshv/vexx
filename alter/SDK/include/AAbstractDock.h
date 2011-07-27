#ifndef BASEDOCK_H
#define BASEDOCK_H

#include "../SDK/AGlobal"
#include "QDockWidget"

class APlugin;
class QMenu;

class ALTER_EXPORT AAbstractDock : public QObject
    {
    Q_OBJECT
public:
    XProperty( QString, name, setName );
    XProperty( bool, hasVerticalTitlebar, setVerticalTitlebar );
    XProperty( APlugin *, app, setApp );

public:
    enum TitleBarPosition { Vertical, Horizontal };
    enum Area { Left=1, Right=2, Top=4, Bottom=8 };
    AAbstractDock( );
    virtual ~AAbstractDock( );
    virtual void added();
    void setTitleBarPosition( TitleBarPosition );
    QWidget *base();
    virtual void menu( QMenu * );
    bool isVisible();

private:
    QWidget *_base;
    };

#endif
