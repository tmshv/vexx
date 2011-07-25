#include "node.h"
#include "gfxscene.h"
#include "APlugin"
#include <QGraphicsSceneMouseEvent>
#include "QDebug"
#include <QTimer>
#include <math.h>
#include "groupNode.h"
#include <QKeyEvent>
#include "utils.h"

// this function works out which input sector of the node the mouse is currently in
bool pointIsInSector(QPointF point, QPointF centre, int sectorNum, int totalSectors)
{
    // point is the mouse position
    // centre is the centre of the node (usually <0,0>)
    // sectorNum is the sector to compare against

    QPointF vectFromCentre = point - centre;  // find the vector from node centre to mouse pos
    float a = vectFromCentre.x();   // the adjacent value
    float o = vectFromCentre.y();   // the opposite value

    float sectorAngleStart = (( M_PI * 2 ) / totalSectors) * sectorNum;     // the start angle of the sector
    float sectorAngleEnd = (( M_PI * 2 ) / totalSectors) * (sectorNum + 1);  // the end angle of the sector

    float pointAngle = atan2( -o, -a );                 // the angle the mouse position makes
    pointAngle += ((M_PI * 2) / totalSectors) / 2.0;    // offset the sector by half the angle of the sector

    //       _
    //     / A |
    //   O|-----|O
    //     \ B /
    //
    // O's are input circles
    // A and B are the hit-box segments
    // sectorAngle is 180 degrees
    // offset the angles by 180 / 2 = 90 degrees:
    //       _
    //     / | |
    //   O| A|B |O
    //     \ | /
    //


    if(pointAngle < 0)  // ensure angle is from 0 to 360
    {
        pointAngle = M_PI * 2 + pointAngle;
    }

    // if the mousePoint angle is between the start and end angles of the current segment
    if(pointAngle > sectorAngleStart && pointAngle < sectorAngleEnd)
    {
        return 1;
    }
    return 0;
}

node* node::findNode(QPointF pos, float diff) // finds the node at this point in the scene
{
    // scene() returns a pointer to the QGraphicsScene, which can then search for QGraphicsItems, using the items() function
    QList <QGraphicsItem*> connectNodes = scene()->items(pos, Qt::IntersectsItemBoundingRect, Qt::DescendingOrder);

    foreach(QGraphicsItem * it, connectNodes) // for each QGraphicsItem found
    {
        if(it != this) // If it's not me
        {
            node* n = qgraphicsitem_cast <node*>( it ); // convert it to a node pointer.  Returns 0 if the current QGraphicsItem cannot be cast to a node
            if(n != 0)  // if it is a node,
            {
                Q_ASSERT( dynamic_cast <node *>(it) ); // ensure that it really is a node
                if(findDist(n->pos(), pos) < n->mRad + diff) // make sure the mouse pointer is really inside the node, not just in the boundingRect.  findDist is in utils.h
                {
                    return n; // return the found node
                }
            }
        }
    }
    return NULL;
}

int node::nodeContainsPoint(QPointF in) // improved selection
{
    if(findDist(in, this->scenePos()) < this->mRad) // if the mouse is inside the node
    {
        return 1; // return 1, i.e. have hit node
    }
    for(int i = 0; i < mConnectList.size(); i++) // loop through my connections
    {
        if(mConnectList[i].poly.containsPoint(in, Qt::OddEvenFill)) // if the point intersects any of them
        {
            return 2; // return 2, i.e have hit connection
        }
    }
    return 0; // otherwise, nothing hit.
}

node::node(nodeEntity* nodeEntityIn ) : mProperties( 0 ) //constructor
{
    mGfxDetails = 0;
    this->setFlag(QGraphicsItem::ItemIsMovable, 1); // this item can move
    this->setFlag(QGraphicsItem::ItemIsSelectable, 1); // and can be selected
    mNodeEntity = nodeEntityIn; // store the pointer to the nodeEntity

    mNodeEntity->setDrawNode( this ); // each nodeEntity also stores a pointer to its drawing node

    // connect mNodeEntity::onConnection() to node::onConnection()
    // connect mNodeEntity::onConnection(), XProperty::entityAdded() and XProperty::entityRemoved() to node::resort()
    // connect mNodeEntity::entityChanged() to node::entityChange()
    // whenever the database node, mNodeEntity, changes, reflect those changes in the draw node (e.g. colour, position)

    connect( mNodeEntity, SIGNAL(onConnection(APropertyDevice*,AProperty*)), this, SLOT(onConnection(APropertyDevice*,AProperty*)) ); // for non-gui connections
    connect( mNodeEntity, SIGNAL(onConnection(APropertyDevice*,AProperty*)), this, SLOT(resort()) ); // the nodes re-shuffle their Z-Ordering when a new node is connected
    connect( mNodeEntity->array(), SIGNAL(entityAdded(AEntity*)), this, SLOT(resort()) ); // or added
    connect( mNodeEntity->array(), SIGNAL(entityRemoved(AEntity*)), this, SLOT(resort()) ); // or removed
    connect( mNodeEntity, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChange(APropertyDevice*,AProperty*)) );


    // reconnect signals to nodes technically connected to my output output.  Needed for when the QGraphicsScene is rebuilt
    nodeEntityList nodeList = mNodeEntity->connectedNodes("output");
    for(int i = 0; i < nodeList.size(); i++)
    {
        if(nodeList[i] != NULL)
        {
            connect(nodeList[i], SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChange(APropertyDevice*,AProperty*)) );
        }
    }

    // sort out inputs
    FOREACHCONST( getEntity()->properties(), i ) // loop through all the properties
    {
        if(i.value()->attributes().contains("Linkable") && i.value()->name() != "output") // for each property that is not an output (therefore an input)
        {
            input newInput;                     // create an input
            newInput.data = i.value()->name();  // store the name
            mInputList.push_back(newInput);     // add it to the list
        }
    }

    mRad = 30 + (mInputList.size() * 5); // Radius is dependent on the number of inputs
    // rebuild the bounding Rectangle, included the node fully, and the nametag.
    mBRect = QRectF(-mRad, -mRad, mRad * 2 + QFontMetrics(QFont()).width(mNodeEntity->name()), mRad * 2);

    // set bools and ints to default.
    mMoving = FALSE;
    mConnecting = FALSE;
    mInputsAreOpen = FALSE;
    mHoverOverNode = NULL;
    mHighlightInput = -1;

    // set my position to the position of my mNodeEntity (for rebuilding the gfxscene)
    XVector3D pos( nodeEntityIn->property("nodeEntity/pos")->toVector3D() );
    setPos( pos.toPointF() );
    setZValue( pos.z() );

    // set my colour to the colour of my mNodeEntity (for rebuilding the gfxscene)
    setColour(mNodeEntity->property("nodeEntity/colour")->toColour());
}

void node::updateShowInputs()
{
    // called after rebuilding the scene.  Opens the inputs of those that ought to be open.
    mAlwaysShowInputs = mNodeEntity->property("alwaysShowInputs")->toBool();

    if(mAlwaysShowInputs)
    {
        openInputs();
    }
}


node::~node()
{
    mNodeEntity->setDrawNode( 0 );
}

QRectF node::boundingRect() const
{
       return mBRect; // for hit detection
}

// PAINT //////////////////////////////////////////////////////////////////////////////////

void node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
{
    mBRect = QRectF(-mRad - 10, -mRad - 10, (mRad * 2) + 20, (mRad * 2) + 20);

    // Draw viewing ring
    if( mNodeEntity->viewNode() != 0 ) // if this node is being viewed
    {
        painter->setPen( Qt::transparent );
        if( mNodeEntity->viewNode() == 1 )
            {
            painter->setBrush( QColor( 24, 49, 82, 150 ) );
            }
        else
            {
            painter->setBrush( QColor( 150, 24, 49, 150 ) );
            }
        painter->drawEllipse(QPoint(0, 0), mRad + 15, mRad + 15);
        mBRect = mBRect.unite( QRectF(-mRad - 13, -mRad - 13, (mRad * 2) + 26, (mRad * 2) + 26));
    }

    // Draw connections:
    QPointF inputPos;
    QColor endCol;

    mConnectList.clear();
    propertyList propList = mNodeEntity->connectedProperties("output");
    for( int i = 0; i < propList.size(); i++ ) // loop through all the properties connected to my output
    {
        if(propList[i] != NULL)
        {
            AProperty* prop = propList[i]; // store the current property

            //find the node that the connected property belongs to
            APropertyDevice* pdevice = propList[i]->parent(); // get the parent of the property
            nodeEntity* pnode = static_cast <nodeEntity*> (pdevice); // convert it to a nodeEntity

            if(pnode != 0) // if the static_cast to a nodeEntity was successful
            {
                connection newConnect; // make a new connection

                //loop through the found node's inputList to find the position of the property I am connected to
                node* dnode = pnode->drawNode(); // get the draw node

                if( dnode )
                {
                    for(int i = 0; i < dnode->mInputList.size(); i++) // loop through its inputs
                    {
                        QString inputName = dnode->mInputList[i].data;
                        QString propName = prop->name();

                        if(inputName == propName) // if this is the connected input
                        {
                            inputPos = dnode->mInputList[i].pos + pnode->property("nodeEntity/pos")->toVector3D().toPointF() - mNodeEntity->property("nodeEntity/pos")->toVector3D().toPointF();
                            endCol = pnode->property("nodeEntity/colour")->toColour().toLDRColour();
                            newConnect.connectedProperty = propName; // store the name of the connected property for easy disconnect later
                            break;
                        }
                    }
                }
                // draw the connection

                // store the connection info
                QPolygonF poly = drawConnection(painter, inputPos, mOutColour, endCol, 0); // draw the connection, which returns the drawn polygon
                mBRect = mBRect.united( poly.boundingRect() );                              // add it to the bounding rect
                newConnect.poly = mapToScene(poly);                 // store in scene coords to save repeatedly converting later
                newConnect.connectedNode = pnode;                   // store the connected node for easy disconnect later

                // push the new connection back onto the list
                mConnectList.push_back(newConnect);

            }
        }
    }

    if(mConnecting)
    {
        mBRect = mBRect.united( drawConnection(painter, mMousePos, mOutColour, QColor(Qt::white), 1).boundingRect() ); // draw dragged connections
        openInputs(); // when connecting, show my inputs
    }

    // draw selection ring
    if(isSelected())
    {
        QColor col(Qt::yellow);
        col.setAlphaF(0.75);
        QPen pen(col, 3);
        mBRect = mBRect.united(QRectF(-mRad - 13, -mRad - 13, (mRad * 2) + 26, (mRad * 2) + 26));
        painter->setPen(pen);
    }
    else
    {
        painter->setPen(Qt::transparent);
    }

    // draw bypassed nodes as grey
    if(mNodeEntity->property("bypass")->toBool())
    {
        // OUTER RING
        painter->setBrush(Qt::darkGray); // draw it as dark grey
        painter->drawEllipse(QPoint(0, 0), mRad, mRad);
    }
    else
    {
        // OUTER RING
        painter->setBrush(mOutColour);  // otherwise draw it as its real colour
        painter->drawEllipse(QPoint(0, 0), mRad, mRad);

    }

    painter->setPen(Qt::transparent);
    painter->setBrush(mColour);

    if(mInputsAreOpen) // draw labels on inputs only if they are open
    {
        if(mInputList.size()) // only draw input labels if it has inputs
        {
            mGfxDetails->setDrawLabels(TRUE);
        }
        painter->setOpacity(0.5);
    }
    else
    {
        mGfxDetails->setDrawLabels(FALSE);
    }

    // INNER RING
    painter->drawEllipse(QPoint(0, 0), 20, 20);
    painter->setOpacity(1);

    // Set brush for input Rings
    painter->setPen(Qt::transparent);

    //draw Inputs
    for(int i = 0; i < mInputList.size(); i++) // loop through each input
    {

        QPointF topLeft = mInputList[i].pos - QPointF(20, 20);
        QPointF bottomRight = mInputList[i].pos + QPointF(20, 20);
        mBRect = mBRect.united(QRectF(topLeft, bottomRight)); // add the input circle rectangles to the bounding rectangle

        QPen pen(Qt::transparent, 3);

        if(mHighlightInput == i) // is this input is highlighted
        {
            pen.setColor(Qt::yellow);
        }
        if(mInputsAreOpen == FALSE) // don't draw highlight ring if inputs are closed
        {
            pen.setColor(Qt::transparent);
        }

        painter->setPen(pen);
        painter->drawEllipse(QRectF(topLeft, bottomRight));  // draw the input circle
    }

    mBRect = mBRect.united(mGfxDetails->getBRect());
    mBRect = mBRect | childrenBoundingRect();

    if( mBRect != mBRectOld )
    {
        prepareGeometryChange();
    }
    mBRectOld = mBRect;
}


QPolygonF node::drawConnection(QPainter *painter, QPointF pos, QColor startCol, QColor endCol, bool draggingConnection )
{
    int start = mRad;
    int end = 20; // the end is always an input, which has radius of 20

    if(draggingConnection)
    {
        end = 1; // if dragging, draw to a point
    }

    float lineLength = findLength(pos); // find the length of the vector from my centre to the mouse position.  findLength is defined in utils.h

    float midPoint1 = (lineLength / 5.0); // the fist mid point is 1/5 along the line
    float midPoint2 = (lineLength - midPoint1); // the second mid point is 4/5 along the line      P1--M1--|--|--M2--P2

    QPainterPath connectionPath;
    connectionPath.moveTo(0, start);  // move to 0 in x, and the node radius in y.  The top of the node.

    float thickness = 0;

    // Draw the curve horizontally first.

    connectionPath.cubicTo(QPointF(midPoint1, thickness), QPointF(midPoint2, thickness), QPointF(lineLength, end)); // cubic bezier curve 1
    connectionPath.lineTo(lineLength, -end);                                                                        // line to next start point
    connectionPath.cubicTo(QPointF(midPoint2, -thickness), QPointF(midPoint1, -thickness), QPointF(0, -start));     // cubic bezier curve 2

    // now rotate it by the required amount

    QTransform rot;
    rot.rotateRadians( atan2(pos.y(), pos.x()) ); // find the angle and rotate by it

    QPointF startGrad(start, 0);           // the start pos for the gradient
    QPointF endGrad(lineLength - end, 0);  // the end pos for the gradient
    startGrad = rot.map(startGrad);         // rotate the 2 points
    endGrad = rot.map(endGrad);

    QLinearGradient grad(startGrad, endGrad); // make a gradient along the line
    grad.setColorAt(0, startCol);             // colour the start (this node)
    grad.setColorAt(1, endCol);               // colour the end (connecting node or mouse pointer)

    // convert it to a polygon and draw it with the gradient
    QPolygonF poly = connectionPath.toFillPolygon(rot);
    painter->setBrush(grad);
    painter->setPen(Qt::transparent);
    painter->drawPolygon(poly);

    return poly;
}

void node::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
    // sets this node to the view node
    if( event->button() == Qt::LeftButton )
    {
        if( mNodeEntity->type() == groupNode::identifier() && event->modifiers() == Qt::ControlModifier )
            {
            mNodeEntity->array()->setWorkingPath( mNodeEntity->location() );
            }
        else
            {
            int id( 1 );
            if( event->modifiers() == Qt::ShiftModifier )
                {
                id = 2;
                }
            mNodeEntity->requestSetViewNode( id );
            scene()->update();
            }
    }
}

void node::centre()
{
    gfxview *view( qobject_cast <gfxview *> (scene()->views().front()) );

    if( view )
    {
        // set pos to centre of screen
        QPointF pos;
        pos.setX(view->rect().width()/2);
        pos.setY(view->rect().height()/2);

        // get transformation matrix and invert it (viewMatrix moves the scene, so invert to move to view)
        QTransform viewMatrix = view->getTransform();
        viewMatrix = viewMatrix.inverted();

        // map point to matrix
        pos = viewMatrix.map(pos);

        // set position
        QVector3D vectPos;
        vectPos.setX(pos.x());
        vectPos.setY(pos.y());
        mNodeEntity->property("nodeEntity/pos") = vectPos;
    }
}

void node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    float dist = findDist(QPointF(0, 0), event->pos());  // find the length of the vector from my centre to the mouse pos

    if( event->button() == Qt::LeftButton )  // if left mouse pressed
    {
        mMousePos = event->pos();
        if( ( dist < 20 ||                                                              // if (inner circle selected
             mapRectToScene(mGfxDetails->currentTextRect()).contains(event->scenePos()) ||
             mGfxDetails->thumbnailRect().contains( mMousePos ) ) &&   // OR nametag selected)
             (event->modifiers()&Qt::AltModifier) == FALSE)                             // AND alt not pressed
        {
            if(!isSelected())
            {
                //deselect everything is shift not pressed
                if( (event->modifiers()&Qt::ShiftModifier) == FALSE )
                {
                    QList <QGraphicsItem*> allNodes = scene()->items();
                    foreach(QGraphicsItem* It, allNodes) // loop through all nodes
                    {
                        It->setSelected(0);             // deselect them
                    }
                }
                setSelected(1);  // set this node to selected
            }

            mMoving = TRUE;  // indicates the node is now being moved
        }
        else if(dist < mRad) // if the outer ring has been pressed
        {
            mConnecting = TRUE;  // begin connecting
        }
        else // as the node has not been clicked, now check to see if a connection has been clicked
        {
            bool hasSelectedConnection = FALSE;
            for(int i = 0; i < mConnectList.size(); i++) // loop through all connections
            {
                QPointF mousePos = event->scenePos();

                if( mConnectList[i].poly.containsPoint(mousePos, Qt::OddEvenFill) )  // if the mouse has clicked a connection
                {
                    // get the connected node:
                    nodeEntity* connectedNodeEnt = mConnectList[i].connectedNode;
                    node* connectedNode = connectedNodeEnt->drawNode();

                    if( connectedNode )
                    {
                        if( findDist(mousePos, connectedNode->scenePos()) > connectedNode->getRad() ) // ensure I have not clicked that actual connected node
                        {
                            connectedNodeEnt->disconnect(mConnectList[i].connectedProperty, mNodeEntity, "output" ); // disconnect the node
                            mConnecting = TRUE;
                            hasSelectedConnection = TRUE;
                            update();
                        }
                    }
                }
            }
            if(!hasSelectedConnection) // if blank space has been clicked
            {
                event->ignore(); // pass mouse events back up to gfxview
            }
        }
    }
    else if( event->button() == Qt::RightButton && dist < mRad ) // if right clicking the node
    {
        propertiesEvent( event );  // open the properties
    }
    else
    {
        event->ignore();
    }
}

void node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mMoving) // if moving
    {
        // loop through all selected items
        QList<QGraphicsItem *> items( scene()->QGraphicsScene::selectedItems() );
        for(int i = 0; i < items.size(); i++)
        {
            // update their positions
            node *n( qgraphicsitem_cast<node *> (items[i]) );
            n->setPos(n->pos() + event->scenePos() - event->lastScenePos());
            n->mGfxDetails->setPos(n->pos());
        }
    }
    else if(mConnecting) // if connecting
    {
        mMousePos = event->pos(); // store mouse pos to avoid recalculating

        // if mouse not over hoverOverNode, or hoverOverNode does not exist:
        if( mHoverOverNode == NULL || (findDist(event->scenePos(), mHoverOverNode->scenePos())) > mHoverOverNode->mRad + 20 )
        {
            node* n = findNode(event->scenePos(), 20); // find a new node under the mouse

            if(n) // if a node has been found...
            {
                // ensure that inputs are closed on old hoverOverNode
                if(mHoverOverNode != NULL) // make sure it exists
                {
                    mHoverOverNode->closeInputs(); // close old hoverOverNode
                    mHoverOverNode->mHighlightInput = -1;
                }
                mHoverOverNode = n;            // store new found node instead
                mHoverOverNode->openInputs();  // then open the new inputs
            }
            else // if no node was found, just close the old hoverOverNode
            {
                // ensure that inputs are closed on old hoverOverNode
                if(mHoverOverNode != NULL) // make sure it exists
                {
                    mHoverOverNode->closeInputs();
                    mHoverOverNode->mHighlightInput = -1;
                }
            }
        }
        else // if the mouse is over the hoverOverNode
        {
            mHoverOverNode->openInputs(); // open its inputs

            // find which input should be highlighted
            mHoverOverNode->mHighlightInput = -1;  // set highlight to nothing

            int size = mHoverOverNode->mInputList.size();
            for(int i = 0; i < size; i++) // loop through hoverOverNode's input
            {
                if(pointIsInSector(event->scenePos(), mHoverOverNode->pos(), i, size )) // if point is in current sector...
                {
                    mHoverOverNode->mHighlightInput = i; // set this as the input to highlight
                }
            }
        }
    }

    update();  // redraw this node

    if(mHoverOverNode != NULL)
    {
        mHoverOverNode->update();  // redraw hoverOverNode
    }
}

void node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(mConnecting)
    {
        node* n;
        // guard against null pointer
        if(mHoverOverNode != NULL)
        {
            n = mHoverOverNode;
        }
        else
        {
            n = findNode(event->scenePos(), 20);
        }

        if(n) // if there is a node to connect to
        {
            if(findDist(event->scenePos(), n->scenePos()) < n->mRad + 20) // if mouse is inside hoverOverNode.  +20 to accommodate for inputs
            {
                if( n != NULL && !n->mInputList.isEmpty() )                // check if node has inputs to connect to
                {
                    int size = n->mInputList.size();
                    for(int i = 0; i < size; i++) // loop through the node's inputs
                    {
                        // get input position;
                        QPointF inputCentre = n->pos() + n->mInputList[i].pos;

                        if(pointIsInSector(event->scenePos(), n->pos(), i, size )) // if released in this input
                        {
                            n->getEntity()->connect(n->mInputList[i].data, mNodeEntity, "output" ); // make a connection
                            break;
                        }
                    }
                }
            }
        }
        mConnecting = FALSE; // stop connecting
        if(mHoverOverNode != NULL)
        {
            mHoverOverNode->closeInputs();  // close the inputs
            mHoverOverNode->mHighlightInput = -1;
            mHoverOverNode = NULL; // remove the pointer
        }
        closeInputs();
        update();
    }
    else if(mMoving)
    {
        mMoving = FALSE;
    }
}

void node::setColour( XColour col )
{
    col.setA( 1.0 );
    mColour = col.toLDRColour(); // set inner circle colour
    mOutColour = QColor::fromHsv( mColour.hue(), mColour.saturation(), xClamp( mColour.value() - 50, 0, 255 ) ); // set the outer ring colour
    if( mGfxDetails )
    {
        mGfxDetails->setColour( mColour ); // set the gfxdetails too
    }
}

void node::setGfxDetails(nodeGraphicalDetails* in) // set the gfxdetails (nametags)
{
    mGfxDetails = in;
    mGfxDetails->setColour( mColour );
    mGfxDetails->setPos( pos() );
}

// returning functions:

int node::getRad()
{
    return mRad;
}

QList <input> node::getInputList()
{
    return mInputList;
}

nodeEntity* node::getEntity()
{
    return mNodeEntity;
}

QColor node::outputColour() const
{
    return mOutColour;
}

void node::openInputs()
{
    if(!mInputsAreOpen) // only open inputs once
    {
        float angle = M_PI * 2 / mInputList.size(); // get the angles around the circles

        for(int i = 0; i < mInputList.size(); i++)
        {
            // trig:  rad * cos(T), rad * sin(T)
            mInputList[i].pos = QPointF(-cos(angle * i) * mRad, -sin(angle * i) * mRad); // move each circle out
        }
        mInputsAreOpen = TRUE;
        scene()->update( pos().x() - 500, pos().y() - 500, 1000, 1000 );  // clean up gfx glitches outside bounding rect
    }
}

void node::closeInputs()
{
    if(mInputsAreOpen && (mAlwaysShowInputs == FALSE)) // don't close the inputs if alwaysShowInputs has been set
    {
        for(int i = 0; i < mInputList.size(); i++)
        {
            mInputList[i].pos = QPointF(0,0);  // reset all the input positions to node centre
        }
        mInputsAreOpen = FALSE;
        scene()->update( pos().x() - 500, pos().y() - 500, 1000, 1000 );  // clean up gfx glitches outside bounding rect
    }
}

//SLOTS

void node::entityChange( APropertyDevice *dev, AProperty * data ) // called when a connected node changes in any way
{
    if( dev != this->mNodeEntity && data->name() == "nodeEntity/pos" ) // if the position has been updated
    {
        update(); // not strictly necessary, as this node should update by default anyway due to intersecting bounding rectangles, but this is safer.
    }
    else
    {
        // on name change
        if(data->name() == "entity/name")
        {
            mGfxDetails->update(); // update display of the nametag
        }
        else if(data->name() == "nodeEntity/pos") // if the mNodeEntity has changed position for some reason
        {
            setPos( data->value().toVector3D().toPointF() );
            mGfxDetails->setPos( this->pos() );
        }
        else if(data->name() == "nodeEntity/colour")
        {
            setColour(mNodeEntity->property("nodeEntity/colour")->toColour());
            update();
        }
    }

}

void node::onConnection( APropertyDevice* dev, AProperty * data)
{
    if(data->attributes().contains("singleInput") )
    {
        connect(dev, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChange(APropertyDevice*,AProperty*)) );
        update();
    }
}

int node::type() const
{
    return Type;
}

void node::resort()  // resorts the Z-Ordering
{
    QVector3D pos = mNodeEntity->property("nodeEntity/pos")->toVector3D();
    if(mNodeEntity->connectedInputs().size() == 0 ) // unconnected nodes will always be at the top of the network
    {
        pos.setZ(10000);
        mNodeEntity->property("nodeEntity/pos") = pos;
        setZValue(10000);
    }
    else
    {
        float inZ = 10000;
        float outZ = -10000;

        // find the lowest Z value in the nodes connected to my inputs
        propertyList inList = mNodeEntity->connectedInputs();
        for(int i = 0; i < inList.size(); i++)
        {
            float z = inList[i]->value().toVector3D().z();
            if( z < inZ)
            {
                inZ = z;
            }
        }

        // find the highest Z value in the nodes connected to my outputs
        nodeEntityList outList = mNodeEntity->connectedNodes("output");
        for(int i = 0; i < outList.size(); i++)
        {
            float z = outList[i]->property("nodeEntity/pos")->toVector3D().z();
            if( z > outZ )
            {
                outZ = z;
            }
        }

        // inZ and outZ now represent the narrowest range of Z values either side of this node

        float z = (inZ + outZ) / 2.0; // set my Z value to exactly inbetween the two

        // update pos
        pos.setZ(z);
        mNodeEntity->property("nodeEntity/pos") = pos;

        setZValue(z);
    }
}

// these 3 functions deal with floating properties dialogs and are not my implementation:
void node::toggleProperties()
{
    if( !mProperties )
    {
        mProperties = new floatingProperties( mGfxDetails );
        mProperties->setVisible( TRUE );
        mNodeEntity->property( "showProperties" ) = TRUE;
    }
    else
    {
        mProperties->setVisible( !mProperties->isVisible() );
        mNodeEntity->property( "showProperties" ) = FALSE;
    }
}

void node::propertiesEvent( QGraphicsSceneMouseEvent *event )
{
    if( !mProperties )
    {
        mProperties = new floatingProperties( mGfxDetails );
        mProperties->setVisible( TRUE );
        mNodeEntity->property( "showProperties" ) = TRUE;
        if( event->modifiers() == Qt::ControlModifier )
        {
            mProperties->setMode( floatingProperties::Developer );
        }
    }
    else
    {
        if( event->modifiers() == Qt::ControlModifier )
        {
            if( mProperties->mode() == floatingProperties::Properties )
            {
                mProperties->setMode( floatingProperties::Developer );
            }
            else
            {
                mProperties->setMode( floatingProperties::Properties );
            }
            mProperties->setVisible( TRUE );
        }
        else
        {
            mProperties->setMode( floatingProperties::Properties );
            mProperties->setVisible( !mProperties->isVisible() );
            mNodeEntity->property( "showProperties" ) = FALSE;
        }
    }
}

void node::setDisplayPropertiesFromEntity()
{
    if( mNodeEntity->property( "showProperties" )->toBool() && !mProperties )
    {
        mProperties = new floatingProperties( mGfxDetails );
    }

    if( mProperties )
    {
        mProperties->setVisible( mNodeEntity->property( "showProperties" )->toBool() );
    }
}


// deals with setting alwaysShowInputs on and off
void node::toggleInputs()
{
    if(mAlwaysShowInputs)
    {
        mAlwaysShowInputs = FALSE;
        closeInputs();
    }
    else
    {
        mAlwaysShowInputs = TRUE;
        openInputs();
    }
    mNodeEntity->property("alwaysShowInputs") = mAlwaysShowInputs;
}


