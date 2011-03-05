#ifndef JLIB_GRAPHICS_PIXEL_IMAGE_H
#define JLIB_GRAPHICS_PIXEL_IMAGE_H

#include "graphics/pixel/graphics.pixel.types.h"
#include "graphics/pixel/graphics.pixel.texture.h"

class QImage;

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

class JLIB_EXPORT image
    {
public:
    enum xGravity { LEFT, CENTER, RIGHT };
    enum yGravity { TOP, MIDDLE, BOTTOM };
    struct colourPacket
        {
        JFLOAT r;
        JFLOAT g;
        JFLOAT b;
        JFLOAT a;
        };
    image( );
    image( file::ioFile file );
    image( const texture & );
    image( const image &in );
    image( unsigned int x, unsigned int y, math::colour4 c=math::colour4(0,0,0,0) );
    ~image( );
    image &operator=( const image & );
    bool isValid() const;
    unsigned int width() const;
    unsigned int height() const;
    JFLOAT ratio() const;
    void setWidth( unsigned int x, xGravity gravity=CENTER );
    void setHeight( unsigned int y, yGravity gravity=MIDDLE );
    //void drawString( string character, int x, int y, font face=font(), colour4 colour=colour4() );
    void drawImage( image, int, int );
    QImage toQImage() const;
    void saveFile( file::ioFile ) const;
    inline colourPacket *at( unsigned int x, unsigned int y )
        { return _data + x + ( y * _width ); }
    inline const colourPacket *at( unsigned int x, unsigned int y ) const
        { return _data + x + ( y * _width ); }
private:
    unsigned int _width;
    unsigned int _height;
    colourPacket *_data;
    //font *currentFont;
    //void drawCharacter( unsigned int xPos, unsigned int yPos, int character, colour4 colour=colour4() );
    };

END_JLIB_GRAPHICS_PIXEL_NAMESPACE

/*namespace old
{
//! Utility class to load an image and provide references to pixel data
class JLIB_EXPORT image
    {
public:
    //! Holds expection information
    ExceptionInfo *exception;
    //! an Image pointer to image data
    Image *img;
    //! an ImageInfo pointer to image info data
    ImageInfo *info;

    //! Initiate an image
    image( int _w, int _h, colour4 _c );
    //! Initiate an image from a filename
    image( file::ioFile in );
    //! Destroy image data
    ~image();

    //! Draw Text on the Image
    void annotate( string text, float x, float y, double _p, string _f, colour4 _c );
    //! Get a reference to the 0,0 pixel, holding access to the whole image so we can read it all out
    PixelPacket *getReference( );
    //! Get a reference to the x,y pixel with access to w x h pixels
    PixelPacket *getReference( int x, int y, int w, int h );
    };
}*/

#endif

