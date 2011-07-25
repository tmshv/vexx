#include "nodedock.h"
#include "nodegraphicaldetails.h"
#include <QGraphicsView>
#include "gfxscene.h"
#include "gfxview.h"
#include "directoryListing.h"
#include "persistantProperties.h"
#include "ALog"

Q_DECLARE_METATYPE( AEntityLocation )

nodeDock::nodeDock()
{
    mLayout = new QVBoxLayout(base());
    mLayout->setContentsMargins(0, 0, 0, 0); // tiny margins

    base()->setLayout(mLayout); // point the the base dock to the layout

    mView = new gfxview(base());
    mScene = new gfxscene(base());

    connect( mScene, SIGNAL(refreshScene()), this, SLOT(rebuildGui()) );

    connect( mScene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()) );
    connect( mScene, SIGNAL(changed(QList<QRectF>)), this, SLOT(nodesMoved(QList<QRectF>)) );

    mView->setScene(mScene);
    mLayout->addWidget(mView); // add the gfxview as a widget to the layout

    _appSelecting = false;
}

void nodeDock::added()
{
    connect( app(), SIGNAL(entityAdded(AEntity*)), this, SLOT(centreNode(AEntity*)) );
    connect( app(), SIGNAL(entityRemoved(AEntity*)), this, SLOT(removeNode(AEntity*)) );
    connect( app(), SIGNAL(entityRemoved(AEntity*)), this, SLOT(entityRemoved(AEntity*)) );

    connect( app(), SIGNAL(selectionChanged()), this, SLOT(appSelectionChanged()) );
    connect( app(), SIGNAL(insertState()), this, SLOT(rebuildGui()) );
    connect( app(), SIGNAL(cleanState()), this, SLOT(rebuildGui()) );
    connect( app(), SIGNAL(openState()), this, SLOT(rebuildGui()) );
    connect( app(), SIGNAL(workingPathChanged()), this, SLOT(rebuildGui()) );

    mView->setApp(app());

    mScene->addItem( new propertiesWidget( app(), mView ) );

    qRegisterMetaType<AEntityLocation>();

    directoryListing *dir( new directoryListing( app(), mView ) );
    mScene->addItem( dir );
    connect( dir, SIGNAL(changeDirectory(AEntityLocation)), this, SLOT(changeDirectory(AEntityLocation)), Qt::QueuedConnection );
}

void nodeDock::changeDirectory( AEntityLocation loc )
    {
    app()->setWorkingPath( loc );
    }

void nodeDock::centreNode( AEntity *ent ) // places the node in the centre of the view
{
    nodeEntity *node( ent->castTo<nodeEntity*>() );

    if( node )
    {
        connect( node, SIGNAL(requestNetworkRedraw()), mScene, SLOT(update()), Qt::UniqueConnection );
        if( qFuzzyCompare( node->property("nodeEntity/pos")->toVector3D(), QVector3D( 0, 0, 0 ) ) )
        {
            // set pos to centre of screen
            QPointF pos;
            pos.setX(mView->rect().width()/2);
            pos.setY(mView->rect().height()/2);

            // get transformation matrix and invert it (viewMatrix moves the scene, so invert to move to view)
            QTransform viewMatrix = mView->getTransform();
            viewMatrix = viewMatrix.inverted();

            // map point to matrix
            pos = viewMatrix.map(pos);

            // set position
            QVector3D vectPos;
            vectPos.setX(pos.x());
            vectPos.setY(pos.y());
            node->property("nodeEntity/pos") = vectPos;
        }
        //Rebuild gui from here, rather than direct signal so only rebuild on NODE added, not just any entity
        rebuildGui();
    }
}

void nodeDock::rebuildGui( )
{
    if( !app()->inserting() )
        {
        // when nodeEntities are added, add them to the gfxscene
        clear();

        AEntitySearch nodeSearch = app()->search().findDirectChildren( app()->workingPath() ).findHasProperty("nodeEntity/isNode"); // find all existing nodes
        foreach (AEntity* ent, nodeSearch) // for every node
        {
            nodeEntity* nodeEnt = static_cast<nodeEntity*>(ent); // cast it to a nodeEntity.

            node* n = new node( nodeEnt ); // make a new drawer node, and pass in the nodeEntity

            nodeList << n; // store the drawer node in a list

            nodeGraphicalDetails* g = new nodeGraphicalDetails( app(), nodeEnt ); // create a new gfxdetails
            n->setGfxDetails(g);                                                                 // and add it to the node

            mScene->addItem(n); //add the items to the scene
            mScene->addItem(g);

            n->updateShowInputs(); // show the inputs of nodes set to alwaysShowInputs
            n->setDisplayPropertiesFromEntity(); // updates the floating properties pane
        }
    }
    mView->update();
}

void nodeDock::entityRemoved( AEntity * )
{
    // may need to disconnect here?
}

void nodeDock::appSelectionChanged()
    {
    if( !_appSelecting )
        {
        _appSelecting = true;
        foreach( QGraphicsItem *item, mScene->selectedItems() )
            {
            item->setSelected( 0 );
            }

        foreach( AEntity *ent, app()->selection() )
            {
            nodeEntity *node( ent->castTo<nodeEntity*>() );
            if( node && node->drawNode() )
                {
                node->drawNode()->setSelected( true );
                }
            }
        _appSelecting = false;
        }
    }

void nodeDock::selectionChanged()
{
    if( !_appSelecting )
        {
        _appSelecting = true;
        // add any selected items to the app base
        app()->selectClear();
        foreach(QGraphicsItem* item, mScene->selectedItems() )
        {
            app()->selectAdd(static_cast<node*>(item)->getEntity());
        }
        _appSelecting = false;
    }
}

void nodeDock::nodesMoved(const QList<QRectF> & rectList) // inputs the rectangles in which changes have occured
{
    static bool nodeMoved = FALSE;
    if( !nodeMoved )
    {
        nodeMoved = TRUE;
        foreach(QRectF rect, rectList) // foreach rectangle (will only really be one)
        {
            QList<QGraphicsItem*> itemList( mScene->items(rect, Qt::ContainsItemBoundingRect) ); // select all items inside rectangle and store in itemList

            foreach(QGraphicsItem* item, itemList) //for each item
            {
                if(nodeList.contains(item)) // if it is a node
                {
                    node* n = qgraphicsitem_cast<node*>(item);
                    if( n ) // if it is a node
                    {
                        // ensure the nodeEntity is updated to contain the position of the drawNode
                        QVector3D newPos = QVector3D( n->pos().x(), n->pos().y(), n->zValue() );
                        if( !qFuzzyCompare( n->getEntity()->property("nodeEntity/pos")->toVector3D(), newPos ) )
                        {
                            n->getEntity()->property("nodeEntity/pos") = newPos;
                        }
                    }
                }
            }
        }
        nodeMoved = FALSE;
    }
}

void nodeDock::clear()
{
    QSet <QGraphicsItem *> deleteList;
    // delete all the nodes in the dock
    foreach( QGraphicsItem *it, mScene->items() )
    {
        if( it->parentItem() == 0
            && !qgraphicsitem_cast<propertiesWidget*>(it)
            && !qgraphicsitem_cast<persistantProperties*>(it)
            && !qgraphicsitem_cast<floatingProperties*>(it)
            && !qgraphicsitem_cast<directoryListing*>(it) )
        {
            deleteList << it;
        }
    }
    qDeleteAll( deleteList );
}

// this was George's implementation
void nodeDock::removeNode( AEntity *ent )
{
    QSet <QGraphicsItem *> deleteList;
    foreach( QGraphicsItem *item, mScene->items( ) )
    {
        node* n( qgraphicsitem_cast<node*>(item) );
        if( n && n->getEntity() == ent )
        {
            deleteList << n;
            continue;
        }
        nodeGraphicalDetails* nD( qgraphicsitem_cast<nodeGraphicalDetails*>(item) );
        if( nD && nD->getEntity() == ent )
        {
            deleteList << nD;
            continue;
        }
    }
    qDeleteAll( deleteList );
}

