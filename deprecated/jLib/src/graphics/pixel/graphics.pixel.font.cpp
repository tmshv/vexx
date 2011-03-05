#include "graphics/pixel/graphics.pixel.font.h"
#include "QFont"

BEGIN_JLIB_GRAPHICS_PIXEL_NAMESPACE

font::font( string family, unsigned int pH ) : _font( new QFont( QString::fromStdString( family ), pH ) )
    {
    }

font::font( const font &in ) : _font( new QFont( *(reinterpret_cast<QFont *> (in._font)) ) )
    {
    }

font::~font( )
    {
    delete reinterpret_cast<QFont *> (_font);
    }

font &font::operator=( const font &in )
    {
    delete reinterpret_cast<QFont *> (_font);
    _font = new QFont( *(reinterpret_cast<QFont *> (in._font)) );
    return *this;
    }

void font::setSize( unsigned int pH )
    {
    reinterpret_cast<QFont *> (_font)->setPointSize( pH );
    }

unsigned int font::size( ) const
    {
    return reinterpret_cast<QFont *> (_font)->pointSize();
    }

void font::setFamily( string in )
    {
    reinterpret_cast<QFont *> (_font)->setFamily( QString::fromStdString( in ) );
    }

jLib::string font::family( ) const
    {
    return reinterpret_cast<QFont *> (_font)->family().toStdString();
    }

END_JLIB_GRAPHICS_PIXEL_NAMESPACE
