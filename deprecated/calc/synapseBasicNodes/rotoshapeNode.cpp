#include "rotoshapeNode.h"
#include "QPolygonF"
#include "QPainterPath"
#include "QImage"
#include "QPainter"
#include "nodeEntityManipulator.h"
#include "QToolBar"
#include "ASelectionEvent"
#include "QMouseEvent"
#include "utils.h"
#include "graphics/shape/graphics.shape.line.h"
#include "QIcon"

rotoshapeNode::rotoshapeNode() : _mode( Move ), _inserting( -1 )
    {
    INIT_NODE

    // shape property to hold the rotoshape
    addProperty( "shape", XVariantList(), propertyInfo::String, "Shape" );
    property("shape").hide();

    // the node toolbar
    _tools = new QToolBar();

    // add the toolbar actions
    _tools->addAction( QIcon(":/roto/rotoMovePoint.svg"), "Move Point", this, SLOT(movePoint()) );
    _tools->addAction( QIcon(":/roto/rotoAddPoint.svg"), "Add Point", this, SLOT(addPoint()) );
    _tools->addAction( QIcon(":/roto/rotoRemovePoint.svg"), "Remove Point", this, SLOT(removePoint()) );

    // help
    property( "longHelp" ) = "A Rotoshape node allows the user to draw a single arbitrary polygonal shape for use in the composite.";
    property( "shortHelp" ) = "A Rotoshape node allows the user to draw a single arbitrary polygonal shape for use in the composite.";
    }

void rotoshapeNode::rebuildPoly()
    {
    // we set the image poly from the shape property
    QPolygonF poly;

    XVariantList list( property("shape")->toList() );
    foreach( AVariant var, list )
        {
        poly << var.toVector2D().toPointF();
        }

    setPoly( poly );
    }

void rotoshapeNode::initiateManipulators( )
    {
    // clear and init the manipulators from the property
    clearManipulators();

    foreach( AVariant var, property("shape")->toList() )
        {
        addManipulator( nodeEntity::Manipulator( var.toVector2D().toPointF(), nodeEntity::Manipulator::Minimal ) );
        }
    }

void rotoshapeNode::manipulatorChanged( int i )
    {
    // set the property from the manipulators
    XVariantList list( property("shape")->toList() );

    if( list.size() > i )
        {
        // edit the manipulators
        list[i] = XVector2D( getManipulator(i).position() );

        property("shape") = list;

        // rebuild the poly from the property
        rebuildPoly();
        }
    }

void rotoshapeNode::paint( QPainter *pai )
    {
    // paint the entity
    nodeEntity::paint( pai );

    if( manipulators().size() )
        {
        XVariantList list( property("shape")->toList() );
        QPointF prevPt( list.back().toVector2D().toPointF() );

        // paint the lines from manipulator to manipulator
        foreach( AVariant var, list )
            {
            pai->drawLine( prevPt, var.toVector2D().toPointF() );

            prevPt = var.toVector2D().toPointF();
            }
        }
    }

void rotoshapeNode::setPoly( QPolygonF poly )
    {
    // get the top left point
    QPointF pt( poly.boundingRect().left(), -poly.boundingRect().top() );

    // get an image the right size
    QImage image( poly.boundingRect().size().toSize(), QImage::Format_ARGB32 );

    // fill with black
    image.fill( qRgba( 0, 0, 0, 0 ) );

    // paint on the image
    QPainter pai( &image );
    pai.setRenderHint( QPainter::Antialiasing, true );

    // move so we paint from the top left of the image
    poly.translate( -1.0 * poly.boundingRect().topLeft() );

    // paint the poly
    pai.setPen( Qt::transparent );
    pai.setBrush( Qt::white );
    pai.drawPolygon( poly, Qt::WindingFill );

    // setup the image
    setImage( pt, image );

    Q_EMIT needsUpdate();
    }

QToolBar *rotoshapeNode::toolbar()
    {
    // our toolbar for the roto buttons
    return _tools;
    }

void rotoshapeNode::selectionEvent( ASelectionEvent *event )
    {
    // when were selected reset the mode to move
    if( event->mode() == ASelectionEvent::Unselected )
        {
        _mode = Move;
        }
    }

void rotoshapeNode::addPoint()
    {
    _mode = Add;
    }

void rotoshapeNode::removePoint()
    {
    _mode = Remove;
    }

void rotoshapeNode::movePoint()
    {
    _mode = Move;
    }

bool rotoshapeNode::canUse( QMouseEvent *event )
    {
    // can we use the mouse event
    if( event->button() == Qt::LeftButton )
        {
        // if were in point adding mode
        if( _mode == Add )
            {
            // get the current shape
            _inserting = -1;
            XVariantList list( property("shape")->toList() );

            if( list.size() > 2 )
                {
                // a map of distances to indexes
                QMap <float, int> insertMap;
                int x( 0 );

                // fill the map
                jLib::math::triple mousePt( XVector2D( event->pos() ).toVector3D() );
                jLib::math::triple oldPt( list.back().toVector2D().toVector3D() );
                foreach( XVariant var, list )
                    {
                    // create a shape-line
                    jLib::math::triple thisPt( var.toVector2D().toVector3D() );
                    jLib::graphics::shape::line thisLine( oldPt, thisPt - oldPt );

                    // test for collision with the mousePt, if it collides, insert to the list
                    float collision( thisLine.collision( mousePt ) );
                    if( collision <= 1.0 && collision >= 0.0 )
                        {
                        insertMap.insert( ( mousePt - thisLine.sample( collision ) ).length(), x );
                        }
                    x++;
                    oldPt = thisPt;
                    }
                if( insertMap.size() )
                    {
                    // if the map has size, insert it into the shape at the correct (closest) point
                    list.insert( insertMap.values()[0], XVector2D( event->pos() ) );
                    _inserting = insertMap.values()[0];
                    }
                }
            else
                {
                // just insert in the list
                _inserting = list.size();
                list << XVector2D( event->pos() );
                }

            // save the shape and rebuild the poly
            property("shape") = list;
            rebuildPoly();
            return true;
            }
        else if( _mode == Remove )
            {
            // remove the point
            return true;
            }
        }
    return false;
    }

void rotoshapeNode::use( QMouseEvent *event )
    {
    // we need a point index and
    if( _mode == Add && _inserting >= 0 )
        {
        // if its a move
        if( event->type() == QEvent::MouseMove )
            {
            // change the point
            XVariantList list( property("shape")->toList() );

            list[_inserting] = XVector2D( event->pos() );

            property("shape") = list;
            rebuildPoly();
            }
        else if( event->type() == QEvent::MouseButtonRelease )
            {
            // otherwise stop the moving
            _inserting = -1;
            }
        }
    else if( _mode == Remove && event->type() == QEvent::MouseButtonRelease )
        {
        // if removing, test collision with the points and remove from the list
        XVariantList list( property("shape")->toList() );

        int x=0;
        foreach( XVariant var, list )
            {
            if( findDist( event->pos(), var.toVector2D().toPointF() ) < 4 )
                {
                // found a point close enough, remove from the list
                list.removeAt( x );
                break;
                }
            x++;
            }

        property("shape") = list;
        rebuildPoly();
        }
    }

void rotoshapeNode::relocated()
    {
    // rebuild the image when the roto is reloaded
    rebuildPoly();
    }
