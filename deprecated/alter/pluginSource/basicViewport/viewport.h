#ifndef APPVIEWPORT_H
#define APPVIEWPORT_H

class appViewport;

#include "AGlobal"
#include "ALog"
#include "AEntityArray"
#include "background.h"
#include "boundingBox.h"
#include "AAbstractDock"
#include "viewportTool.h"

class viewportEntity;

#define find3DEntities findHasProperty( "viewport/object" )

class viewport : public jLib::jqt::viewportWidgetBase
    {
    Q_OBJECT
    jROProperty( APlugin *, app )
    JLIB_QT_MANIPULATOR_WIDGET
public:
    viewport( QWidget *, APlugin * );
    ~viewport();

    void addTool( viewportTool * );
    void removeTool( viewportTool * );

    void refresh();
    void onSelectionChange();
    void geometryChanged();

signals:
    void contextActivate( );

public slots:
    void update();

protected:
    void paintGL();

private slots:
    void selectionChanged();
    void entityAdded( AEntity * );
    void entityRemoved( AEntity * );
    void entityChanged( APropertyDevice *, AProperty * );

private:
    void refreshHeirachy();
    QSet <viewportEntity *> _entities;
    QSet <viewportTool *> _tools;
    void refreshBBoxes();
    void clearSelectionBoxes();
    jLib::vector <boundingBox *> _boxes;
    bool _requestRefresh;
    bool _geometryChanged;
    background _background;
    };

#endif
