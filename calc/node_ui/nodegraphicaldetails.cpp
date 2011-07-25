#include "nodegraphicaldetails.h"
#include "QPainter"
#include "QIcon"
#include "node.h"

nodeGraphicalDetails::nodeGraphicalDetails(APlugin *app, nodeEntity* in) : _app( app )
{
    mNodeEntity = in;
    setZValue(20000);
    bRect = QRectF(-10,-10,20,20);
    drawLabels = FALSE;
}
nodeGraphicalDetails::~nodeGraphicalDetails()
{

}

QRectF nodeGraphicalDetails::getBRect()
{
    return bRect;
}

void nodeGraphicalDetails::setDrawLabels(bool in)
{
    drawLabels = in;
}

void nodeGraphicalDetails::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
{
    _thumbRect = QRectF();
    if(!drawLabels)
    {
        QString nametag(mNodeEntity->name());
        QFontMetrics font(painter->font());
        int fontWidth = font.width(nametag);


        painter->setPen(Qt::transparent);
        painter->setBrush( mColour );
        QRectF rect( -10, -20, fontWidth + 35, 25 );
        painter->drawRoundedRect( rect, 12, 12 );
        _currentTextRect = rect;

        // Draw the thumbnail if possible
        QPixmap thumb( mNodeEntity->thumbnail() );


        if( !thumb.isNull() )
        {
            if( thumb.width() > rect.width() )
            {
                thumb = thumb.scaledToWidth( rect.width() - 10 );
            }
            float top( rect.top() - 10 - thumb.height() );
            _thumbRect = QRectF( rect.right() - thumb.width() - 10, top, thumb.width() + 10, rect.center().y() - top + 2 );
            painter->drawRoundedRect( _thumbRect, 5, 5 );
            painter->fillRect( _thumbRect.left()+5, top+5, thumb.width(), thumb.height(), Qt::black );
            painter->drawPixmap( _thumbRect.left() + 5, top + 5, thumb );
            bRect |= _thumbRect;
        }

        if( mColour.lightness() > 100 )
            {
            painter->setPen(Qt::black);
            }
        else
            {
            painter->setPen(Qt::white);
            }
        QRectF textRect( 5, -20, fontWidth + 20, 25 );
        painter->drawText(textRect, Qt::AlignCenter, nametag);

        bRect |= textRect;

        QPixmap icon( mNodeEntity->icon().pixmap( 16, 16 ) );
        if( icon.size() != QSize( 16, 16 ) && !icon.isNull() )
        {
            icon = icon.scaled( 16, 16 );
        }
        painter->drawPixmap( -4, -16, icon );
    }
    else
    {
        node* dnode = mNodeEntity->drawNode();
        if( dnode )
        {
            for(int i = 0; i < dnode->getInputList().size(); i++)
            {
                QString nametag(dnode->getInputList()[i].data);
                QPointF topLeft(dnode->getInputList()[i].pos);

                QFontMetrics font(painter->font());
                int fontWidth = font.width(nametag);

                QRectF textRect( topLeft.x() - (fontWidth / 2) - 10, topLeft.y() -15, fontWidth + 20, 25 );

                // draw Rect
                painter->setPen(Qt::transparent);
                painter->setBrush( mColour );
                QRectF rect( textRect.topLeft().x() + 5, textRect.topLeft().y(), fontWidth + 10, 25 );

                painter->drawRoundedRect( rect, 12, 12 );

                // draw text
                if( mColour.lightness() > 100 )
                {
                    painter->setPen(Qt::black);
                }
                else
                {
                    painter->setPen(Qt::white);
                }

                painter->drawText(textRect, Qt::AlignCenter, nametag);
                bRect = bRect.united(textRect);

            }
        }
    }
    if( bRect != oldBRect )
    {
        prepareGeometryChange();
        oldBRect = bRect;
    }
}

QRectF nodeGraphicalDetails::boundingRect() const
{
    return bRect | childrenBoundingRect();
}

void nodeGraphicalDetails::setColour( QColor in )
{
    mColour = in;

    if( scene() )
        {
        update();
        }
}

QColor nodeGraphicalDetails::colour() const
{
    return mColour;
}

int nodeGraphicalDetails::type() const
{
    return Type;
}

nodeEntity* nodeGraphicalDetails::getEntity()
{
    return mNodeEntity;
}

APlugin *nodeGraphicalDetails::app()
{
    return _app;
}

QRectF nodeGraphicalDetails::currentTextRect() const
    {
    return _currentTextRect;
    }

QRectF nodeGraphicalDetails::thumbnailRect() const
    {
    return _thumbRect;
    }
