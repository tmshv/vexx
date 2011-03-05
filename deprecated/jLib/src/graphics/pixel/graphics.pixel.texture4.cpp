#include "graphics/pixel/graphics.pixel.texture4.h"
#include "jLib.glHeader.h"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

texture4::texture4() : texUnit( 0 ), img( 0 ), name(""), width( 0 ), height( 0 ) {;}

/*texture4::texture4( old::image &c ) : texUnit( 0 ), img( 0 ), name(""), width( 0 ), height( 0 )
    { loadImage( c ); }*/

texture4::texture4( image &c ) : texUnit( 0 ), img( 0 ), name(""), width( 0 ), height( 0 )
    { loadImage( c ); }

texture4::texture4( file::ioFile _f ) : texUnit( 0 ), img( 0 ), name( _f.getFilename() ), width( 0 ), height( 0 )
    {
    jAssert( _f.exists() );
    image magickImg( name.fileFullName );
    loadImage( magickImg );
    }
texture4::texture4( string _f ) : texUnit( 0 ), img( 0 ), name( _f ), width( 0 ), height( 0 )
    {
    jAssert( file::ioFile::exists( _f ) );
    image magickImg( name.fileFullName );
    loadImage( magickImg );
    }
texture4::~texture4()
    {
    if( img != 0 )
        {
        glDeleteTextures( 1, &img );
        }
    }

void texture4::load( )
    {
    glActiveTexture( GL_TEXTURE0+texUnit );
    glBindTexture( GL_TEXTURE_2D, img );
    }

void texture4::loadImage( image &in )
    {
    jAssert( in.isValid() );

    if( img != 0 )
        {
        glDeleteTextures( 1, &img );
        }

    width = in.width();
    height = in.height();

    float *tempBuf = new float[ 4 * width * height ];

    glGenTextures( 1, &img );
    glBindTexture( GL_TEXTURE_2D, img );

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    for( unsigned int y=0; y<height; y++ )
        {
        for( unsigned int x=0; x<width; x++ )
            {
            struct image::colourPacket *here = in.at( x, y );
            int base = 4 * ( x + y * width );
            tempBuf[ base ] = here->r;
            tempBuf[ base + 1 ] = here->g;
            tempBuf[ base + 2 ] = here->b;
            tempBuf[ base + 3 ] = here->a;
            }
        }


    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, (const GLvoid *)tempBuf );

    delete [] tempBuf;
    }

/*void texture4::loadImage( old::image &c )
    {
    char *tempBuf;

    if( img != 0 )
        {
        glDeleteTextures( 1, &img );
        }


    tempBuf = (char *)malloc(3*c.img->columns*c.img->rows);

    glGenTextures( 1, &img );
    glBindTexture( GL_TEXTURE_2D, img );

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );


    PixelPacket *pointer = c.getReference( 0, 0, c.img->columns, c.img->rows );
    unsigned int pos=0;
    for( unsigned int y=0; y<c.img->rows; y++ )
        {
        //cout<<"Row"<<y<<endl;
        for( unsigned int x=0; x<c.img->columns; x++ )
            {
            tempBuf[pos++]=((pointer+(y*c.img->columns)+x)->red)>>8;
            tempBuf[pos++]=((pointer+(y*c.img->columns)+x)->green)>>8;
            tempBuf[pos++]=((pointer+(y*c.img->columns)+x)->blue)>>8;
            }
        }


    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, c.img->columns, c.img->rows, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *)tempBuf );

    free(tempBuf);

    width = c.img->columns;
    height = c.img->rows;
    }*/

END_JLIB_GRAPHICS_PIXEL_NAMESPACE
