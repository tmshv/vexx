#include "iimage.h"
#include "QDomElement"
#include "QImage"
#include "QFileInfo"

#include "QPainter"

#include "QDebug"

IImage::IImage( QString directory, QDomElement myTag, QDomDocument *doc ) : _tag( myTag ), _dir( directory ), _doc( doc )
    {
    _preparing = 0;
    _valid = false;
    }

IImage::~IImage()
    {
    _tag.removeAttribute("placeHolder");
    }

QString IImage::filename() const
    {
    return _tag.attribute("filename");
    }

bool IImage::isPreparingThumbnail() const
    {
    return _preparing == 1;
    }

void IImage::prepareThumbnail( int size )
    {
    if( _preparing == 0 )
        {
        _preparing = 1;

        QImage im( _dir + "/" + _tag.attribute("filename") );

        if( !im.isNull() )
            {
            _thumbnail = im.scaled( QSize( size, size ), Qt::KeepAspectRatio );
            _valid = true;
            }
        else
            {
            _thumbnail = QImage( size, size, QImage::Format_ARGB32_Premultiplied );
            _thumbnail.fill( qRgba( 0, 0, 0, 0 ) );
            QPainter pai( &_thumbnail );

            pai.setPen( Qt::white );
            pai.drawText( _thumbnail.rect(), Qt::AlignCenter, "Invalid Image: " + _tag.attribute("filename") );
            _valid = false;
            }
        _preparing = 0;
        }
    }

bool IImage::thumbnailReady() const
    {
    return _preparing == 0 && !_thumbnail.isNull();
    }

QImage IImage::thumbnail() const
    {
    return _thumbnail;
    }

void IImage::parse()
    {
    while( _tag.childNodes().size() )
        {
        _tag.removeChild( _tag.firstChild() );
        }
    _tag.removeAttribute("placeHolder");
    }
