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
    addProperty( "shape", XVariantList(), propertyInfo::String, "Shape" );
    property("shape").hide();

    _tools = new QToolBar();

    _tools->addAction( QIcon(":/roto/rotoMovePoint.svg"), "Move Point", this, SLOT(movePoint()) );
    _tools->addAction( QIcon(":/roto/rotoAddPoint.svg"), "Add Point", this, SLOT(addPoint()) );
    _tools->addAction( QIcon(":/roto/rotoRemovePoint.svg"), "Remove Point", this, SLOT(removePoint()) );

    property( "longHelp" ) = "A Rotoshape node allows the user to draw a single arbitrary polygonal shape for use in the composite.";
    property( "shortHelp" ) = "A Rotoshape node allows the user to draw a single arbitrary polygonal shape for use in the composite.";
    }

void rotoshapeNode::rebuildPoly()
    {
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
    clearManipulators();

    foreach( AVariant var, property("shape")->toList() )
        {
        addManipulator( nodeEntity::Manipulator( var.toVector2D().toPointF(), nodeEntity::Manipulator::Minimal ) );
        }
    }

void rotoshapeNode::manipulatorChanged( int i )
    {
    XVariantList list( property("shape")->toList() );

    if( list.size() > i )
        {
        list[i] = XVector2D( getManipulator(i).position() );

        property("shape") = list;

        rebuildPoly();
        }
    }

void rotoshapeNode::paint( QPainter *pai )
    {
    nodeEntity::paint( pai );
    if( manipulators().size() )
        {
        XVariantList list( property("shape")->toList() );
        QPointF prevPt( list.back().toVector2D().toPointF() );
        foreach( AVariant var, list )
            {
            pai->drawLine( prevPt, var.toVector2D().toPointF() );

            prevPt = var.toVector2D().toPointF();
            }
        }
    }

void rotoshapeNode::setPoly( QPolygonF poly )
    {
    QPointF pt( poly.boundingRect().left(), -poly.boundingRect().top() );

    QImage image( poly.boundingRect().size().toSize(), QImage::Format_ARGB32 );
    image.fill( qRgba( 0, 0, 0, 0 ) );
    QPainter pai( &image );
    pai.setRenderHint( QPainter::Antialiasing, true );

    poly.translate( -1.0 * poly.boundingRect().topLeft() );

    pai.setPen( Qt::transparent );
    pai.setBrush( Qt::white );
    pai.drawPolygon( poly, Qt::WindingFill );

    setImage( pt, image );

    Q_EMIT needsUpdate();
    }

QToolBar *rotoshapeNode::toolbar()
    {
    return _tools;
    }

void rotoshapeNode::selectionEvent( ASelectionEvent *event )
    {
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
    if( event->button() == Qt::LeftButton )
        {
        if( _mode == Add )
            {
            _inserting = -1;
            XVariantList list( property("shape")->toList() );

            if( list.size() > 2 )
                {
                QMap <float, int> insertMap;
                int x( 0 );
                jLib::math::triple mousePt( XVector2D( event->pos() ).toVector3D() );
                jLib::math::triple oldPt( list.back().toVector2D().toVector3D() );
                foreach( XVariant var, list )
                    {
                    jLib::math::triple thisPt( var.toVector2D().toVector3D() );
                    jLib::graphics::shape::line thisLine( oldPt, thisPt - oldPt );

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
                    list.insert( insertMap.values()[0], XVector2D( event->pos() ) );
                    _inserting = insertMap.values()[0];
                    }
                }
            else
                {
                _inserting = list.size();
                list << XVector2D( event->pos() );
                }

            property("shape") = list;
            rebuildPoly();
            return true;
            }
        else if( _mode == Remove )
            {
            return true;
            }
        }
    return false;
    }

void rotoshapeNode::use( QMouseEvent *event )
    {
    if( _mode == Add && _inserting >= 0 )
        {
        if( event->type() == QEvent::MouseMove )
            {
            XVariantList list( property("shape")->toList() );

            list[_inserting] = XVector2D( event->pos() );

            property("shape") = list;
            rebuildPoly();
            }
        else if( event->type() == QEvent::MouseButtonRelease )
            {
            _inserting = -1;
            }
        }
    else if( _mode == Remove && event->type() == QEvent::MouseButtonRelease )
        {
        XVariantList list( property("shape")->toList() );

        int x=0;
        foreach( XVariant var, list )
            {
            if( findDist( event->pos(), var.toVector2D().toPointF() ) < 4 )
                {
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
    rebuildPoly();
    }
