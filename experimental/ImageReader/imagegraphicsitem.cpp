#include "imagegraphicsitem.h"
#include "QPainter"
#include "QThread"
#include "QGraphicsDropShadowEffect"

#include "QDebug"


class ThumbnailThread : public QThread
    {
    IImage &_img;
    float _size;
public:
    ThumbnailThread( IImage &img, float size ) : _img( img ), _size( size ) { }
    void run()
        {
        _img.prepareThumbnail( _size );
        }
    };

QImage ImageGraphicsItem::_invalid( ":/pleaseWaitClock.svg" );
QImage ImageGraphicsItem::_shadow( ":/shadow.png" );

ImageGraphicsItem::ImageGraphicsItem( IImage img ) : _img( img ), _size( 200 )
    {
    setFlags( QGraphicsItem::ItemIsSelectable );
    }

void ImageGraphicsItem::setSize( float size )
    {
    _size = size;
    }

QRectF ImageGraphicsItem::boundingRect() const
    {
    return QRectF( 0, 0, _size, _size );
    }

QPainterPath ImageGraphicsItem::shape() const
    {
    QPainterPath path;
    path.addRect( _shapeRect );
    return path;
    }

void ImageGraphicsItem::thumbnailReady()
    {
    update();
    }

void ImageGraphicsItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
    {
    painter->setRenderHints( QPainter::Antialiasing, true );

    QImage image;
    if( _img.thumbnailReady() )
        {
        QPointF centre( boundingRect().toRect().center() );
        image = _img.thumbnail();

        _shapeRect = QRect( centre.toPoint() - (QPoint(image.size().width(),image.size().height())/2), image.size() );
        }
    else
        {
        if( !_img.isPreparingThumbnail() )
            {
            ThumbnailThread *thread( new ThumbnailThread( _img, _size-20 ) );
            connect( thread, SIGNAL(finished()), this, SLOT(thumbnailReady()) );
            connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()) );
            thread->start();
            }
        QPointF centre( boundingRect().toRect().center() );
        image = _invalid;

        _shapeRect = QRect( centre.toPoint() - (QPoint(image.size().width(),image.size().height())/2), image.size() );
        }

        {
        QImage shdw( _shadow );

        shdw = shdw.scaled( QSize( image.width() + 40, shdw.height() ), Qt::KeepAspectRatio );

        painter->drawImage( _shapeRect.bottomLeft() - QPoint( 20, 0.55 * shdw.height() ), shdw );
        }

    if( isSelected() )
        {
        QRectF selected( _shapeRect.adjusted( -3, -3, 3, 3 ) );
        QLinearGradient grad( selected.topLeft(), selected.bottomLeft() );
        grad.setColorAt( 0, QColor( 250, 230, 150 ) );
        grad.setColorAt( 0, QColor( 225, 175, 80 ) );

        painter->setPen( Qt::transparent );
        painter->setBrush( grad );
        painter->drawRoundedRect( selected, 3, 3 );

        painter->fillRect( _shapeRect, QColor( 50, 50, 50 ) );
        }

    painter->drawImage( _shapeRect, image );
    }
