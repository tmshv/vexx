#include "nodeWidget.h"
#include "jLib.h"
#include "QPainter"

#define ROUNDRADIUS 2
#define SELECTED_COLOUR QColor( 241, 209, 55 )
#define SELECTED_DARK_COLOUR QColor( 122, 103, 9 )

#define NODEPROPERTYHEIGHT 25

inline QPainterPath makeRoundedPath( QRectF titlebarRect )
    {
    QPainterPath ret;
    ret.addRoundedRect( titlebarRect, ROUNDRADIUS, ROUNDRADIUS );
    return ret;
    }

nodeWidget::nodeWidget()
    {
    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable );
    }

void nodeWidget::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
    {
    QString title( "Hello" );
    QList <QString> properties;

    properties << "Thing" << "lalala" << "bouind" << "hahaha";

    QRectF originalRect( titlebarRect( painter->font() ) );

    _titlePath = QPainterPath();
    _titlePath.addRect( originalRect );

    QRectF titleRect( originalRect );
    titleRect.setHeight( titleRect.height() + ( ROUNDRADIUS * 2 + 1 ) );

    QRectF contentsRect( titleRect );
    contentsRect.setY( originalRect.height() );
    contentsRect.setHeight( originalRect.height() + NODEPROPERTYHEIGHT*( properties.size() - 1 ) - 2*ROUNDRADIUS - 4 );

    QPainterPath titleBar( makeRoundedPath( titleRect ) );
    QPainterPath contents( makeRoundedPath( contentsRect ) );

    QPen outlinePen( Qt::black, 3 );
    if( isSelected() )
        {
        outlinePen.setColor( SELECTED_COLOUR );
        }
    QPainterPath globalLine( makeRoundedPath( titleRect.unite( contentsRect ) ) );
    painter->setPen( outlinePen );
    painter->drawPath( globalLine );
    painter->fillPath( titleBar, titleGradient( titleRect ) );


    painter->fillPath( contents, contentsBrush( contentsRect ) );

    QColor separatingLineColor( Qt::black );
    if( isSelected() )
        {
        separatingLineColor = SELECTED_COLOUR;
        }
    separatingLineColor.setAlpha( 128 );
    QPen separatingLinePen( separatingLineColor );
    QPen whitePen( Qt::white );

    painter->setPen( whitePen );
    painter->drawText( originalRect, Qt::AlignCenter, title );

    QRectF textRect( titleRect );
    textRect.moveTop( textRect.top() + originalRect.height() - 2*ROUNDRADIUS - 2 );
    FOREACH( properties, prop )
        {
        painter->setPen( whitePen );
        painter->drawText( textRect, Qt::AlignCenter, *prop );
        if( &(*prop) != &(properties.back()) )
            {
            painter->setPen( separatingLinePen );
            painter->drawLine( titleRect.x(), textRect.y()+NODEPROPERTYHEIGHT+4, titleRect.x()+titleRect.width(), textRect.y()+NODEPROPERTYHEIGHT+4 );
            }
        textRect.moveTop( textRect.top() + NODEPROPERTYHEIGHT );
        }

    _bounds = titleRect.unite( contentsRect );
    _bounds.adjust( -1, -1, 1, 1 );
    }

QBrush nodeWidget::titleGradient( QRectF titleRect ) const
    {
    QLinearGradient titleGrad( QPointF( 0, 0 ), QPointF( 0, titleRect.height() ) );
    if( isSelected( ) )
        {
        titleGrad.setColorAt( 0, SELECTED_COLOUR );
        titleGrad.setColorAt( 1, SELECTED_DARK_COLOUR );
        }
    else
        {
        titleGrad.setColorAt( 0, QColor( 150, 150, 150 ) );
        titleGrad.setColorAt( 1, QColor( 50, 50, 50 ) );
        }
    return titleGrad;
    }

QBrush nodeWidget::contentsBrush( QRectF titleRect ) const
    {
    QLinearGradient titleGrad( QPointF( 0, 0 ), QPointF( 0, titleRect.height() ) );
    if( isSelected( ) )
        {
        titleGrad.setColorAt( 0, QColor( 232, 214, 126 ) );
        titleGrad.setColorAt( 1, QColor( 158, 147, 91) );
        }
    else
        {
        titleGrad.setColorAt( 0, QColor( 195, 195, 195 ) );
        titleGrad.setColorAt( 1, QColor( 155, 155, 155 ) );
        }
    return titleGrad;
    }

QRectF nodeWidget::boundingRect( ) const
    {
    return _bounds;
    }

QPainterPath nodeWidget::shape() const
    {
    return _titlePath;
    }

int nodeWidget::minimumWidth( QFont ) const
    {
    return 150;
    }

QRectF nodeWidget::titlebarRect( QFont font ) const
    {
    return QRectF( 0, 0, minimumWidth( font ), 30 );
    }
