#ifndef JLIB_GRAPHICS_FONT_H
#define JLIB_GRAPHICS_FONT_H


#include "graphics/pixel/graphics.pixel.types.h"
#include "file/file.ioFile.h"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

class JLIB_EXPORT font
    {
public:
    font( string family="", unsigned int pointHeight=20 );
    font( const font &in );
    font &operator=( const font & );
    ~font( );
    void setSize( unsigned int pointHeight=12.0 );
    unsigned int size( ) const;
    void setFamily( string );
    jLib::string family( ) const;

private:
    void *_font;
    friend class textLine;
    };

END_JLIB_GRAPHICS_PIXEL_NAMESPACE


#endif
