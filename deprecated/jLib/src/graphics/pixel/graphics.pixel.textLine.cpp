#include "graphics/pixel/graphics.pixel.textLine.h"
#include "QImage"
#include "QFontMetrics"
#include "QTextLayout"
#include "QPainter"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

textLine::textLine( string t, font f )
    {
    _text = t;
    _font = f;
    }

void textLine::setText( string t )
    {
    _text = t;
    }

void textLine::setFont( const font &f )
    {
    _font = f;
    }

const font &textLine::getFont( ) const
    {
    return _font;
    }

string textLine::getText( ) const
    {
    return _text;
    }

image textLine::getImage( math::colour4 fg )
    {
    QFontMetrics fontMetrics( *reinterpret_cast<QFont *>(_font._font) );
    int leading = fontMetrics.leading();
    QTextLayout textLayout( QString::fromStdString( _text ) );

    textLayout.beginLayout();
    QTextLine line = textLayout.createLine( );
    qreal height = leading;
    line.setPosition( QPointF( 0, height ) );
    height = leading + line.height();
    qreal width = line.naturalTextWidth();
    textLayout.endLayout( );

    unsigned int widthI = (int)width, heightI = (int)height;

    QImage pic( widthI, heightI, QImage::Format_ARGB32 );
    QPainter painter( &pic );
    painter.setPen( QColor( (int)(255*fg.r()), (int)(255*fg.g()), (int)(255*fg.b()), (int)(255*fg.a()) ) );
    painter.setFont( *reinterpret_cast<QFont *>(_font._font) );
    painter.drawText( 0, 0, QString::fromStdString( _text ) );

    image ret( widthI, heightI, math::colour4( fg.r(), fg.g(), fg.b(), 0.0 ) );

    QRgb *pointer = (QRgb *)pic.bits();
    for( unsigned int y=0; y<heightI; y++ )
        {
        for( unsigned int x=0; x<widthI; x++ )
            {
            QRgb pix = pointer[ x + (y*widthI) ];
            ret.at( x, y )->r = qRed( pix ) / 255.0f;
            ret.at( x, y )->g = qGreen( pix ) / 255.0f;
            ret.at( x, y )->b = qBlue( pix ) / 255.0f;
            ret.at( x, y )->a = qAlpha( pix ) / 255.0f;
            }
        }
    return ret;
    }

END_JLIB_GRAPHICS_PIXEL_NAMESPACE
