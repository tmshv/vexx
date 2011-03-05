#include "gfxview.h"
#include "QKeyEvent"
#include "node.h"
#include "ALog"
#include "QtPieMenu"
#include "qtpieaction.h"
#include "nodeDB.h"
#include "utils.h"
#include "helpSystem.h"
#include "QUrl"
#include "QDir"
#include "QFile"
#include "QDesktopServices"
#include "QGLFormat"
#include "QTransform"

gfxview::gfxview(QWidget* parent) : QGraphicsView(parent) // constructor
{
    setRenderHints(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor); // allows transformations

#ifdef SYNAPSE_GL_VIEWPORT // toggle to use openGL rendering
    QGLFormat fmt;
    fmt.setSampleBuffers( true );

    setViewport( new QGLWidget( fmt ) );
#endif

    // turn off scroll bars
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    setFocusPolicy( Qt::ClickFocus );

    // set default values
    mScaling = FALSE;
    mTranslating = FALSE;
    mDragSelecting = FALSE;
    bgMap = 0;
    mRootMenu = 0;

    rebuildPieMenu();

    nodeDB::nodeTypesWatcher *watcher( new nodeDB::nodeTypesWatcher( this ) ); // allows the nodeDB to watch this node for when it changes type (under developer tab)
    connect( watcher, SIGNAL(typesChanged()), this, SLOT(rebuildPieMenu()) );  // allows the pie menu to be rebuilt whenever a new node type has been created
}

gfxview::~gfxview( )
    {
    delete bgMap;
    }

void gfxview::rebuildPieMenu()
{
    // delete any existing pie menu
    if( mRootMenu )
    {
        delete mRootMenu;
    }

    // Now create a new menu called Create
    QtPieMenu *createMenu = new QtPieMenu(tr("Create"), this );

    foreach( QString group, nodeDB::nodeGroups() ) // for all of the possible node types there are
    {
        // make submenu for each node group
        QtPieMenu *groupMenu = new QtPieMenu(group, createMenu);

        foreach( nodeDB::nodeType node, nodeDB::listNodeGroup( group )) // for each node in the group...
        {
            // add an element to the group, connected to the create() function
            groupMenu->insertItem( node.name(), this, SLOT(create()), node );
        }

        createMenu->insertItem(groupMenu); // insert this group to the create meny
    }

    // Finally save the createmenu as the root menu.
    mRootMenu = createMenu;
}

void gfxview::resizeEvent(QResizeEvent *) // called when the view is resized
{
    // recalculate the background pixmap

    // delete the old one
    if( bgMap == 0 )
    {
        delete bgMap;
    }

    //make a new one
    bgMap = new QPixmap(width(), height());
    QPainter painter(bgMap);

    // recalculate the radial gradient and store it
    QRectF bgRect(QPointF(0,0), QPointF(width(), height()));

    QRadialGradient grad(bgRect.center(), width());
    grad.setColorAt(0, Qt::darkGray);
    grad.setColorAt(1, Qt::black);
    painter.fillRect(bgRect, grad);
    scene()->update();

    emit viewResized();
}


void gfxview::setApp(APlugin * in)
{
    mApp = in;
}

void gfxview::keyReleaseEvent( QKeyEvent *event )
{
    // Open properties on Control Q, George's implementation
    if( event->key() == Qt::Key_Q && event->modifiers() == Qt::ControlModifier )
    {
        QList<QGraphicsItem *> items( scene()->QGraphicsScene::selectedItems() );

        foreach( QGraphicsItem *it, items )
        {
            node *n( qgraphicsitem_cast<node *> (it) );
            if( n )
            {
                n->toggleProperties();
            }
        }
    } // if Ctrl I is pressed, my implementation
    else if( event->key() == Qt::Key_I && event->modifiers() == Qt::ControlModifier )
    {
        QList<QGraphicsItem *> items( scene()->QGraphicsScene::selectedItems() );

        foreach( QGraphicsItem *it, items )  // loop through all selected items
        {
            node *n( qgraphicsitem_cast<node *> (it) ); // convert to nodes
            if( n )
            {
                n->toggleInputs();  // toggle their inputs
            }
        }
    }// the following code displays help automatically, and is George's implementation
    else if( event->key() == Qt::Key_H && event->modifiers() == Qt::ShiftModifier )
        {
        QList<QGraphicsItem *> items( scene()->QGraphicsScene::selectedItems() );

        // search selected items
        foreach( QGraphicsItem *it, items )
        {
            // if we have a node selected
            node *n( qgraphicsitem_cast<node *> (it) );
            if( n )
            {
                // try to find its associated entity
                nodeEntity *ent( n->getEntity() );

                // create a temporary help file for use in the default browser
                QFile f( QDir::tempPath() + "/Synapse_" + ent->property("nodeEntity/group")->toString() + "_" + ent->property("nodeEntity/type")->toString() + "_docs.html" );
                if( f.open( QFile::WriteOnly ) )
                {
                    f.write( getLongHelp( ent ).toAscii() );

                    // open the browser
                    QDesktopServices::openUrl( QUrl::fromLocalFile( f.fileName() ) );
                    break;
                }
            }
        }
    }
    else
    {
        // default key handling
        QGraphicsView::keyReleaseEvent( event );
    }
}

void gfxview::mouseDoubleClickEvent( QMouseEvent *event )
{
    // early out and return
    if( event->button() != Qt::LeftButton )
    {
        QGraphicsView::mouseDoubleClickEvent( event );
        return;
    }
    QPointF mappedMousePos = mapToScene(event->pos()); // store the mouse position in scene coordinates
    QList <QGraphicsItem*> gfxitems = scene()->items(mappedMousePos);
    bool haveHitTool = FALSE;
    bool haveHitNode = FALSE;
    for(int i = 0; i < gfxitems.size(); i++) // loop through all selected items
    {
        node* n = qgraphicsitem_cast <node*> (gfxitems[i] );
        if(n) // if the item is a node
        {
            if(n->nodeContainsPoint(mappedMousePos)) // and the node has been clicked
            {
                haveHitNode = TRUE;
            }
        }
        else //if the item is not a node
        {
            haveHitTool = TRUE; // a tool has been hit
        }
    }

    if( haveHitTool == FALSE && haveHitNode == FALSE ) // if a node has been hit
    {
        mApp->setWorkingPath( mApp->workingPath().parent() );
    }
    else
    {
        QGraphicsView::mouseDoubleClickEvent( event );
    }
}

void gfxview::mousePressEvent( QMouseEvent *event )
{
    QPointF mappedMousePos = mapToScene(event->pos()); // store the mouse position in scene coordinates
    if( event->button() == Qt::MidButton || event->buttons() == (Qt::LeftButton|Qt::RightButton) )
    {
        mTranslating = TRUE; // begin translating
        mLastMousePos = event->pos(); // store the old mouse position
    }
    else if( event->button() == Qt::RightButton && (event->modifiers()&Qt::AltModifier) ) // if holding down alt, and pressing right click
    {
        mScaling = TRUE; // begin scaling
        mLastMousePos = event->pos(); // store the old mouse position
    }// The following code shows properties on right-click, and is George's implementation
    else if( event->button() == Qt::RightButton )
    {
        QList <QGraphicsItem*> nodes = scene()->items( mappedMousePos );
        if( nodes.size() )
        {
            bool openProperties = FALSE;
            for(int i = 0; i < nodes.size(); i++)
            {
                node* n = qgraphicsitem_cast <node*> ( nodes[i] );
                if(n)
                {
                    if( findDist(n->pos(), mappedMousePos) < n->getRad() )
                    {
                        QGraphicsView::mousePressEvent( event );
                        openProperties = TRUE;
                    }
                }
            }
            if(!openProperties)
            {
                mRootMenuPoint = mappedMousePos;
                mRootMenu->popup(event->globalPos());
            }
        }
        else
        {
            mRootMenuPoint = mappedMousePos;
            mRootMenu->popup(event->globalPos());
            mRootMenuPoint = mappedMousePos;
        }
    }   // the following is my implementation
    else // if left mouse pressed
    {
        QList <QGraphicsItem*> gfxitems = scene()->items(mappedMousePos);
        if(gfxitems.size() == 0) // if nothing has been hit
        {
            mDragSelecting = TRUE; // begin drag selecting
            mSelectRectStart = mappedMousePos;   // store the beginning and end of the selection box
            mSelectRectEnd = mappedMousePos;
        }
        else // need to see if I have hit only the bounding rect of a node, and not the node itself
        {
            bool haveHitTool = FALSE;
            bool haveHitNode = FALSE;
            for(int i = 0; i < gfxitems.size(); i++) // loop through all selected items
            {
                node* n = qgraphicsitem_cast <node*> (gfxitems[i] );
                if(n) // if the item is a node
                {
                    if(n->nodeContainsPoint(mappedMousePos)) // and the node has been clicked
                    {
                        haveHitNode = TRUE;
                    }
                }
                else //if the item is not a node
                {
                    haveHitTool = TRUE; // a tool has been hit
                }
            }

            if(haveHitTool == TRUE) // tools have priority.  If a tool has been selected
            {
                // continue with default implementation for tool controls
                QGraphicsView::mousePressEvent( event );
            }
            else if(haveHitNode == TRUE) // if a node has been hit
            {
                // continue with default implementation and pass mouse events down to the node
                QGraphicsView::mousePressEvent( event );
            }
            else // if nothing was hit at all
            {
                mDragSelecting = TRUE; // begin drag selecting
                mSelectRectStart = mappedMousePos;
                mSelectRectEnd = mappedMousePos;
            }
        }
    }

}

void gfxview::mouseMoveEvent( QMouseEvent *event )
{
    if(mTranslating)
    {
        // translate the view by the amount moved
        float transValx = (event->x() - mLastMousePos.x()); // difference in x
        float transValy = (event->y() - mLastMousePos.y()); // difference in y

        QTransform transMatrix; // identity matrix
        transMatrix.translate(transValx, transValy); // translate by difference
        viewMatrix = viewMatrix * transMatrix;      // multiply by current viewMatrix

        setTransform(viewMatrix); // set the view to use the current matrix
        mLastMousePos = event->pos(); // update the mousePos
        emit viewMoved();
    }
    else if(mScaling)
    {
        float scaleVal = 1 + ((event->x() - mLastMousePos.x()) / 100); // scale down the value by 100
        QPointF transPos(rect().width() / 2, rect().width() / 2);   // ensure scaling occurs in the centre of the view

        float viewScaleVal = viewMatrix.m11(); // element m11 of the matrix contains the scale value

        if( (viewScaleVal <= 7 && scaleVal > 1) || (viewScaleVal >= 0.15 && scaleVal < 1) ) // limit the scaling
        {
            QTransform scaleMatrix, transMatrix, backTransMatrix;
            transMatrix.translate(transPos.x(), transPos.y());  // translate to the middle of the screen
            backTransMatrix.translate(-transPos.x(), -transPos.y());    // translate back

            scaleMatrix.scale(scaleVal, scaleVal);  // scale
            viewMatrix = viewMatrix * backTransMatrix * scaleMatrix * transMatrix;  // mulitply them with the viewMatrix

            setTransform(viewMatrix);   // set the Transform of the view
            mLastMousePos = event->pos();    // update the mousePos
            emit viewScaled(); // tell the floating properties that the view has scaled, so they can compensate (so properties dialogs don't scale)
        }
    }
    else if(mDragSelecting) // if a selection box is being dragged
    {
        mSelectRectEnd = mapToScene(event->pos()); // update the size of the rectangle
        scene()->invalidate(sceneRect(), QGraphicsScene::ForegroundLayer); // redraw the foreground, which contains the drag rectangle
    }
    else
    {
        QGraphicsView::mouseMoveEvent( event );
    }

}

void gfxview::mouseReleaseEvent( QMouseEvent *event )
{
    if(mScaling || mTranslating)
    {
        mScaling = FALSE;
        mTranslating = FALSE;
    }
    else if( mDragSelecting )
    {
        // deselect all items if shift was not pressed
        if(!(event->modifiers()&Qt::ShiftModifier))
        {
            QList <QGraphicsItem*> allNodes = scene()->items();
            for(int i = 0; i < allNodes.size(); i++) // for all nodes
            {
                allNodes[i]->setSelected(0);    // deselect
            }
        }

        QRectF selectionRect(mSelectRectStart, mSelectRectEnd); // the dragged box
        QList <QGraphicsItem*> nodes = scene()->items(selectionRect); // find all items in the box
        for(int i = 0; i < nodes.size(); i++) // loop through selected nodes
        {
            if(selectionRect.contains(nodes[i]->pos())) // if the current node is inside the box
            {
                nodes[i]->setSelected(1); // select it
            }
        }
        mDragSelecting = FALSE;
        scene()->invalidate(sceneRect(), QGraphicsScene::ForegroundLayer); // update the dragSelection box (i.e hide it)
    }
    else
    {
        scene()->invalidate(sceneRect(), QGraphicsScene::ForegroundLayer);
        QGraphicsView::mouseReleaseEvent( event );
    }
}

void gfxview::wheelEvent( QWheelEvent * event )
{
    QPointF transPos = event->pos(); // get the mouse pos to translate to

    QTransform transMatrix, backTransMatrix, scaleMatrix;
    transMatrix.translate(transPos.x(), transPos.y());  // translate to the mouse pos
    backTransMatrix.translate(-transPos.x(), -transPos.y());    // translate back


    float scaleVal = (event->delta() * 0.001) + 1;  //scale
    scaleMatrix.scale( scaleVal, scaleVal );

    float viewScaleVal = viewMatrix.m11();

    if( (viewScaleVal <= 7 && scaleVal > 1) || (viewScaleVal >= 0.15 && scaleVal < 1) ) // only update the scaling if it is not beyond the limits
    {
        viewMatrix = viewMatrix * backTransMatrix * scaleMatrix * transMatrix;
    }
    setTransform(viewMatrix); // update transform

    mLastMousePos = event->pos(); // update mousepos
    emit viewScaled(); // tell the floating properties that the view has scaled, so they can compensate (so properties dialogs don't scale)
}

void gfxview::drawBackground( QPainter *painter, const QRectF & )
{
    //redraw the stored radial gradient pixmap
    painter->setWorldTransform( QTransform() );
    painter->drawPixmap( 0, 0, *bgMap );
}

void gfxview::drawForeground( QPainter *painter, const QRectF & )
{
    if(mDragSelecting)
    {
        // draw a rectangle for the selection box
        QRectF selectionRect(mSelectRectStart, mSelectRectEnd);
        painter->drawRect(selectionRect);
    }
}

QTransform gfxview::getTransform() // returns the transform matrix
{
    return viewMatrix;
}

void gfxview::create()
{
    QtPieAction* pieAction = qobject_cast<QtPieAction*> (sender() );  // the sender is the pie menu that sent the signal
    nodeDB::nodeType type( pieAction->data() ); // send the creation data (name, and group) to the constructor for nodeType
    nodeEntity* newNode = type.createAndInsert( mApp ); // create a new node

    QVector3D point;
    point.setX(mRootMenuPoint.x());
    point.setY(mRootMenuPoint.y());
    newNode->property("nodeEntity/pos") = point; // set the node position to where the mouse was right-clicked
}
