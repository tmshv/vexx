#ifndef VIEWPORTDOCK_H
#define VIEWPORTDOCK_H

#include "AAbstractDock"
#include "viewport.h"

class QMenuBar;

class VIEWPORT_EXPORT viewportDock : public AAbstractDock
    {
    Q_OBJECT
public:
    viewportDock();
    ~viewportDock();
    void added();

    void addTool( viewportTool * );
    void removeTool( viewportTool * );

    void addDrawType( QString );
    void removeDrawType( QString );
    bool shouldDraw( QString );

    void refresh();

signals:
    void showTypesChanged( );
    void contextActivate( );

private:
    viewport *_ui;
    QVBoxLayout *_layout;
    QMenuBar *_menu;
    QMenu *_showMenu;
    QHash <QString, QAction *> _drawTypes;
    };


#endif // VIEWPORTDOCK_H
