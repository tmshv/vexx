#ifndef ROTOSHAPENODE_H
#define ROTOSHAPENODE_H

#include "abstractQImageNode.h"

class rotoshapeNode : public abstractQImageNode
    {
    Q_OBJECT
    NODE_TYPE( rotoshapeNode, "psynapse.placid-acid.com", "Source/Rotoshape", QColor( 204, 204, 204 ) );
public:
    enum ClickMode { Move, Add, Remove };
    rotoshapeNode();

    virtual void initiateManipulators();
    virtual void manipulatorChanged( int );
    virtual void paint(QPainter *);
    virtual QToolBar *toolbar();
    virtual void selectionEvent( ASelectionEvent * );
    virtual void relocated();

    virtual bool canUse(QMouseEvent *);
    virtual void use(QMouseEvent *);

private slots:
    void addPoint();
    void removePoint();
    void movePoint();

private:
    void manipulatorsChanged();
    void rebuildPoly();
    void setPoly( QPolygonF );
    QToolBar *_tools;
    ClickMode _mode;
    int _inserting;
    };

#endif // ROTOSHAPENODE_H
