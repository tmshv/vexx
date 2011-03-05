#ifndef NODE_H
#define NODE_H

class node;

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include "gfxscene.h"
#include "gfxview.h"
#include "nodeEntity.h"
#include "floatingProperties.h"
#include "nodegraphicaldetails.h"

struct input //the structure used for storing information about input circles.
{
    QPointF pos;
    QString data; // stores the name of the property it represents
};

struct connection // stored for disconnection
{
    QPolygonF poly; // used for hit detection
    nodeEntity* connectedNode; // pointer to the node connected to
    QString connectedProperty; // name of the property connected to
};

class node : public QGraphicsObject // the node class
{
Q_OBJECT
public:
    enum { Type = UserType + 1 }; // necessary for the gfxscene to determine what is a node and what isn't

    node(nodeEntity *); // node must be constructed with nodeEntity pointer
    ~node();

    QRectF boundingRect() const; // returns mBRect.  Used by the gfxview for hit detection
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget); // the draw function

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );

    node* findNode(QPointF, float diff = 0); // finds any node at the given location
    void setColour( XColour );
    void setGfxDetails(nodeGraphicalDetails*); // gfxdetails hold the nametag, and input nametags
    void setDisplayPropertiesFromEntity();  // shows the properties, George's implementation
    void updateShowInputs();

    void centre();

    void openInputs();
    void closeInputs();
    void toggleInputs(); // used to set alwaysShowInputs
    void toggleProperties(); // George's implementation

    int nodeContainsPoint(QPointF in); // custom hit detection
    int getRad();
    QList <input> getInputList();
    nodeEntity* getEntity();

    int type() const; // returns the type.  Used by gfxscene to recognise all nodes as this type.
    QColor outputColour() const; // returns mOutColour

private slots:
    void entityChange( APropertyDevice *, AProperty * ); // recieves a signal when a connected node changes
    void onConnection( APropertyDevice *, AProperty * ); // recieves a signal when a connection is made (externally of the GUI)
    void resort(); // fixes Z-Ordering

private:
    void propertiesEvent( QGraphicsSceneMouseEvent *event ); // implemented by George

    QPolygonF drawConnection(QPainter*, QPointF, QColor, QColor, bool mode);

    nodeEntity* mNodeEntity; // the database representation of a node
    QColor mColour;         // inner ring colour
    QColor mOutColour;      // outer ring colour
    nodeGraphicalDetails* mGfxDetails;  // contains the nametags of the node (to ensure they are always drawn on top)

    bool mMoving;           // if moving a node
    bool mConnecting;       // if making a connection
    bool mInputsAreOpen;    // if inputs are open
    bool mAlwaysShowInputs; // if alwaysShowInputs has been set

    int mHighlightInput;    // the input number to highlight.  -1 is none.
    int mRad;               // radius of the node

    node* mHoverOverNode;   // pointer to the node currently hovering over.

    QPointF mMousePos;      // mouse position buffer
    QRectF mBRect;          // bounding rectangle
    QRectF mBRectOld;       // oldbounding rectangle
    floatingProperties *mProperties;

    QList <connection> mConnectList;    // a list of the all the connections coming out of this node, used for disconnect
    QList <input> mInputList;           // a list of the inputs this node has.
};


#endif // NODE_H
