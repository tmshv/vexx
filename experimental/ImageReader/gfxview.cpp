#include "gfxview.h"
#include "QPainter"
#include "QResizeEvent"
#include "iimagelibrary.h"
#include "imagegraphicsitem.h"

#include "QDebug"

gfxView::gfxView( IImageLibrary *lib ) : _library( lib ), _imageSize( 200 )
    {
    setInteractive( true );
    setDragMode( QGraphicsView::RubberBandDrag );

    setScene( &_scene );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    foreach( IImage img, lib->files() )
        {
        _images << new ImageGraphicsItem( img );
        _scene.addItem( _images.back( ) );
        }
    }

gfxView::~gfxView()
    {
    }

QSize gfxView::sizeHint() const
    {
    return QSize( 800, 600 );
    }

void gfxView::resizeEvent( QResizeEvent *event )
    {
    QGraphicsView::resizeEvent( event );

    _imagesPerRow = (int)( viewport()->width() / _imageSize );
    _actualImageSize = (float)viewport()->width() / (float)_imagesPerRow;

    float column = 0;
    float row = 0;
    foreach( ImageGraphicsItem *item, _images )
        {
        item->setSize( _actualImageSize );
        item->setPos( _actualImageSize * column, _actualImageSize * row );

        if( column == _imagesPerRow )
            {
            column = 0;
            row++;
            }
        else
            {
            column++;
            }
        }

    _rows = row+1;
    _scene.setSceneRect( 0, 0, width(), _actualImageSize * _rows );
    }

void gfxView::drawBackground( QPainter *painter, const QRectF &rect )
    {
    painter->fillRect( rect, QColor( 50, 50, 50 ) );
    }
