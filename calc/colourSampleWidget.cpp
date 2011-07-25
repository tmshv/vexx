#include "colourSampleWidget.h"
#include "QPainter"
#include "ALog"

colourSampleWidget::colourSampleWidget( QWidget *parent ) : QWidget( parent )
    {
    // set up our widget dimensions
    setMinimumHeight( 25 );
    setMaximumHeight( 25 );

    setMinimumWidth( 150 );
    setMaximumWidth( 150 );

    // setup strechyness of widget
    setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Fixed );
    }

void colourSampleWidget::paintEvent( QPaintEvent * )
    {
    // create painter
    QPainter pai( this );

    // fill to dark
    pai.fillRect( rect(), palette().color( QPalette::Shadow ) );

    // set up painter font
    QFont fnt( pai.font() );
    fnt.setPixelSize( 8 );
    pai.setFont( fnt );

    // find maximum component, or 1.0 of all below 1.0
    float max( qMax( 1.0f, qMax( qMax( qMax( _col.r(), _col.g() ), _col.b() ), _col.a() ) ) );

    // find the pixel heights of the colour bars, with the maximum being foudn from the max above
    int r( ( _col.r() / max ) * height() ),
        g( ( _col.g() / max ) * height() ),
        b( ( _col.b() / max ) * height() ),
        a( ( _col.a() / max ) * height() );

    // no outline
    pai.setPen( Qt::transparent );

    int rectWidth( width()/4 );

    // draw the rectangles
    pai.setBrush( Qt::red );
    pai.drawRect( 0, height(), rectWidth, -r );

    pai.setBrush( Qt::green );
    pai.drawRect( rectWidth, height(), rectWidth, -g );

    pai.setBrush( Qt::blue );
    pai.drawRect( rectWidth*2, height(), rectWidth, -b );

    pai.setBrush( Qt::white );
    pai.drawRect( rectWidth*3, height(), rectWidth+1, -a );

    // no brush
    pai.setBrush( Qt::transparent );

    // draw a line to indicate the position of 1.0, if there were any values above 1.0
    if( max > 1.0 )
        {
        QColor col( Qt::black );
        col.setAlpha( 64 );
        pai.setPen( col );
        pai.drawLine( 0, height() - ( height() / max ), width(), height() - ( height() / max ) );
        }

    // draw the text float values on the bars
    pai.setPen( Qt::black );
    pai.drawText( 0, 0, rectWidth, height(), Qt::AlignCenter, QString::number( _col.r(), 'g', 3 ) );
    pai.drawText( rectWidth, 0, rectWidth, height(), Qt::AlignCenter, QString::number( _col.g(), 'g', 3 ) );
    pai.drawText( rectWidth*2, 0, rectWidth, height(), Qt::AlignCenter, QString::number( _col.b(), 'g', 3 ) );
    pai.drawText( rectWidth*3, 0, rectWidth, height(), Qt::AlignCenter, QString::number( _col.a(), 'g', 3 ) );

    // draw a frame around the rectandle cause it looks nice
    QColor col( Qt::white );
    col.setAlpha( 128 );
    pai.setPen( col );
    pai.drawRect( QRect( rect().topLeft(), rect().size()-QSize( 1, 1 ) ) );
    }

void colourSampleWidget::setColour( XColour col )
    {
    // set the colour from outside, ands update to reflect this.
    _col = col;
    update();
    }
