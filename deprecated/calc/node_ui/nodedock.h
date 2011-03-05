#ifndef NODEDOCK_H
#define NODEDOCK_H

#include "AAbstractDock"
#include "gfxscene.h"
#include "gfxview.h"

#include <QObject>
#include <QVBoxLayout>

class nodeDock : public AAbstractDock
{
Q_OBJECT
public:
    nodeDock();
    virtual void added();

public slots:
    void selectionChanged();
    void nodesMoved(const QList<QRectF>&);

private slots:
    void rebuildGui( );
    void entityRemoved( AEntity * );
    void centreNode( AEntity * );
    void removeNode( AEntity * );
    void changeDirectory(AEntityLocation);
    void appSelectionChanged();

private:
    void clear();
    gfxscene* mScene;
    gfxview* mView;

    QList<QGraphicsItem*> nodeList;
    QVBoxLayout* mLayout;
    bool _appSelecting;
};

#endif // NODEDOCK_H
