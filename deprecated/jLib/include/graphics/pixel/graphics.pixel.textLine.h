#ifndef JLIB_GRAPHICS_PIXEL_TEXTBOX_H
#define JLIB_GRAPHICS_PIXEL_TEXTBOX_H

#include "graphics/pixel/graphics.pixel.types.h"
#include "graphics/pixel/graphics.pixel.image.h"
#include "graphics/pixel/graphics.pixel.texture4.h"
#include "graphics/pixel/graphics.pixel.font.h"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

class JLIB_EXPORT textLine
    {
public:
    textLine( string text="", font=font() );
    void setText( string );
    void setFont( const font & );
    const font &getFont( ) const;
    string getText( ) const;
    image getImage( math::colour4 );

private:
    string _text;
    font _font;
    };

END_JLIB_GRAPHICS_PIXEL_NAMESPACE

#endif
