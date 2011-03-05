#include "graphics/pixel/graphics.pixel.image.h"
#include "jLib.glHeader.h"
#include "QImage"
#include "QFileInfo"

#ifdef JLIB_OPENEXR_SUPPORT
# include "ImfRgbaFile.h"
# include "ImfArray.h"
#endif


#ifdef JLIB_OPENEXR_SUPPORT
inline bool isExr( const jLib::file::ioFile &file )
    {
    QFileInfo f( QString::fromStdString( file.getFilename() ) );
    return f.suffix().toLower() == "exr";
    }
#endif

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

image::image( ) : _width( 0 ), _height( 0 ), _data( 0 )
    {
    }

image::image( file::ioFile file ) : _width( 0 ), _height( 0 ), _data( 0 )
    {
    if( file.exists() )
        {
        QImage im( QString::fromStdString( file.getFilename() ) );
        im.convertToFormat( QImage::Format_ARGB32 );

        if( !im.isNull() )
            {
            _width = im.width();
            _height = im.height();

            _data = new colourPacket[ _width * _height ];

            const QRgb *pointer = (QRgb *)im.bits();
            for( unsigned int y=0; y<_height; y++ )
                {
                for( unsigned int x=0; x<_width; x++ )
                    {
                    QRgb pix = pointer[ x + (y*_width) ];
                    at( x, y )->r = qRed( pix ) / 255.0f;
                    at( x, y )->g = qGreen( pix ) / 255.0f;
                    at( x, y )->b = qBlue( pix ) / 255.0f;
                    at( x, y )->a = qAlpha( pix ) / 255.0f;
                    }
                }
            }
#ifdef JLIB_OPENEXR_SUPPORT
        else if( isExr( file ) )
            {
            //try exr...
            Imf::RgbaInputFile exr( file.getFilename().c_str() );
            Imath::Box2i dw( exr.dataWindow() );

            _width = dw.max.x - dw.min.x + 1;
            _height = dw.max.y - dw.min.y + 1;

            Imf::Array2D<Imf::Rgba> pixels;
            pixels.resizeErase( _height, _width );

            exr.setFrameBuffer( &pixels[0][0] - dw.min.x - dw.min.y * _width, 1, _width );
            exr.readPixels( dw.min.y, dw.max.y );

            _data = new colourPacket[ _width * _height ];

            for( unsigned int y=0; y<_height; y++ )
                {
                for( unsigned int x=0; x<_width; x++ )
                    {
                    Imf::Rgba &pix = pixels[y][x];
                    at( x, y )->r = pix.r;
                    at( x, y )->g = pix.g;
                    at( x, y )->b = pix.b;
                    at( x, y )->a = pix.a;
                    }
                }
            }
#endif
        }
    }

image::image( const texture &in ) : _width( in.width() ), _height( in.height() ), _data( 0 )
    {
    jRenderDebug( "Construct image from texture "<<&in<<", ["<<_width<<","<<_height<<"]" );

    _data = new colourPacket[ _width * _height ];

    in.bind();
    jRenderDebug( "Extract image from bound texture into "<<_data );
    glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, _data ) GLE;
    in.unbind();
    }

image::image( const image &in ) : _width( in._width ), _height( in._height ), _data( 0 )
    {
    if( isValid() )
        {
        _data = new colourPacket[ _width * _height ];

        for( unsigned int x=0; x<( _width * _height ); x++ )
            {
            _data[x].r = in._data[x].r;
            _data[x].g = in._data[x].g;
            _data[x].b = in._data[x].b;
            _data[x].a = in._data[x].a;
            }
        }
    }

image::image( unsigned int x, unsigned int y, math::colour4 c ) : _width( 0 ), _height( 0 ), _data( 0 )
    {
    if( x > 0 && y > 0 )
        {
        _width = x;
        _height = y;

        _data = new colourPacket[ _width * _height ];

        for( unsigned int x=0; x<( _width * _height ); x++ )
            {
            _data[x].r = c.r();
            _data[x].g = c.g();
            _data[x].b = c.b();
            _data[x].a = c.a();
            }
        }
    }

image &image::operator=( const image &in )
    {
    if( _data != 0 )
        {
        delete [] _data;
        }
    _width = in._width;
    _height = in._height;

    if( isValid() )
        {
        _data = new colourPacket[ _width * _height ];

        for( unsigned int x=0; x<( _width * _height ); x++ )
            {
            _data[x].r = in._data[x].r;
            _data[x].g = in._data[x].g;
            _data[x].b = in._data[x].b;
            _data[x].a = in._data[x].a;
            }
        }
    return *this;
    }

image::~image( )
    {
    if( _data != 0 )
        {
        delete [] _data;
        }
    }

bool image::isValid( ) const
    {
    return _width != 0 && _height != 0;
    }

unsigned int image::width() const
    {
    return _width;
    }

unsigned int image::height() const
    {
    return _height;
    }

JFLOAT image::ratio() const
    {
    jAssert( isValid() );
    return (JFLOAT)_width/(JFLOAT)_height;
    }

void image::setWidth( unsigned int newWidth, xGravity gravity )
    {
    jAssert( newWidth > 0 );
    jAssert( _height > 0 );
    unsigned int oldWidth = _width;
    struct colourPacket *oldData = _data;
    unsigned int offset=0;

    if( gravity == CENTER )
        {
        offset = ( _width - newWidth ) / 2;
        }
    else if( gravity == RIGHT )
        {
        offset = _width - newWidth;
        }


    _width = newWidth;
    _data = new colourPacket[ _width * _height ];

    for( unsigned int y=0; y<_height; y++ )
        {
        for( unsigned int x=0; x<_width; x++ )
            {
            *(at( x, y )) = *(oldData + offset + x + (y*oldWidth));
            }
        }

    delete [] oldData;
    }

void image::setHeight( unsigned int newHeight, yGravity gravity )
    {
    jAssert( newHeight > 0 );
    jAssert( _width > 0 );
    struct colourPacket *oldData = _data;
    unsigned int offset=0;

    if( gravity == MIDDLE )
        {
        offset = ( _height - newHeight ) / 2;
        }
    else if( gravity == BOTTOM )
        {
        offset = _height - newHeight;
        }


    _height = newHeight;
    _data = new colourPacket[ _width * _height ];

    for( unsigned int y=0; y<_height; y++ )
        {
        for( unsigned int x=0; x<_width; x++ )
            {
            *(at( x, y )) = *(oldData + x + ((y+offset)*_width));
            }
        }

    delete [] oldData;
    }

void image::drawImage( image in, int xplace, int yplace )
    {
    jAssert( isValid() );
    jAssert( in.isValid() );

    if( xplace < (int)_width && yplace < (int)_height )
        {
        for( unsigned int y=0; y<in._height; y++ )
            {
            if( yplace + y < _height && yplace + (int)y >= 0 )
                {
                for( unsigned int x=0; x<in._width; x++ )
                    {
                    if( xplace + x < _width && xplace + (int)x >= 0 )
                        {
                        struct colourPacket *here = at( xplace + x, yplace + y );
                        struct colourPacket *there = in.at( x, y );

                        here->r = here->r * ( 1 - there->a ) + there->r * there->a;
                        here->g = here->g * ( 1 - there->a ) + there->g * there->a;
                        here->b = here->b * ( 1 - there->a ) + there->b * there->a;
                        here->a = here->a * ( 1 - there->a ) + there->a * there->a;
                        }
                    }
                }
            }
        }
    }

QImage image::toQImage() const
    {
    QImage im( _width, _height, QImage::Format_ARGB32 );

    QRgb *pointer = (QRgb *)im.bits();
    for( unsigned int y=0; y<_height; y++ )
        {
        for( unsigned int x=0; x<_width; x++ )
            {
            pointer[ x + (y*_width) ] = qRgba( (int)( math::clamp( at( x, y )->r * 255.0f, 0.0, 255.0 ) ),
                                               (int)( math::clamp( at( x, y )->g * 255.0f, 0.0, 255.0 ) ),
                                               (int)( math::clamp( at( x, y )->b * 255.0f, 0.0, 255.0 ) ),
                                               (int)( math::clamp( at( x, y )->a * 255.0f, 0.0, 255.0 ) ) );
            }
        }
    return im;
    }
void image::saveFile( file::ioFile file ) const
    {
    jAssert( isValid() );

#ifdef JLIB_OPENEXR_SUPPORT
    if( isExr( file ) )
        {
        Imf::Rgba *pixels = new Imf::Rgba[ _width * _height ];

        for( unsigned int y=0; y<_height; y++ )
            {
            for( unsigned int x=0; x<_width; x++ )
                {
                Imf::Rgba &pix = pixels[y*_width + x];
                pix.r = at( x, y )->r;
                pix.g = at( x, y )->g;
                pix.b = at( x, y )->b;
                pix.a = at( x, y )->a;
                }
            }

        Imf::RgbaOutputFile exr( file.getFilename().c_str(), _width, _height, Imf::WRITE_RGBA );
        exr.setFrameBuffer( pixels, 1, _width );
        exr.writePixels( _height );
        }
    else
#endif
        {
        QImage im( _width, _height, QImage::Format_ARGB32 );

        QRgb *pointer = (QRgb *)im.bits();
        for( unsigned int y=0; y<_height; y++ )
            {
            for( unsigned int x=0; x<_width; x++ )
                {
                pointer[ x + (y*_width) ] = qRgba( (int)( math::clamp( at( x, y )->r * 255.0f, 0.0, 255.0 ) ),
                                                   (int)( math::clamp( at( x, y )->g * 255.0f, 0.0, 255.0 ) ),
                                                   (int)( math::clamp( at( x, y )->b * 255.0f, 0.0, 255.0 ) ),
                                                   (int)( math::clamp( at( x, y )->a * 255.0f, 0.0, 255.0 ) ) );
                }
            }

        QString fName( QString::fromStdString( file.getFilename() ) );
        if( !im.save( fName ) )
            {
            im.save( fName + ".jpg" );
            }
        }
    }

END_JLIB_GRAPHICS_PIXEL_NAMESPACE
