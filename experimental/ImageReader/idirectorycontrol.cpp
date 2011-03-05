#include "idirectorycontrol.h"
#include "QFile"
#include "QDomDocument"
#include "QDir"
#include "QDebug"

bool isAllowedName( QString name )
    {
    if( name != "IRData.xml" && name != "Thumbs.db" )
        {
        return true;
        }
    return false;
    }

IDirectoryControl::IDirectoryControl( QString dirName )
    {
    _directory = dirName;
    _doc = QDomDocument( "ImageReaderData" );

    QFile file( _directory + "/" + "IRData.xml" );
    if( file.exists() && file.open( QIODevice::ReadOnly ) )
        {
        _doc.setContent( QString::fromUtf8( file.readAll() ) );
        }

    QDomElement docElem = _doc.documentElement();
    if( docElem.isNull() )
        {
        docElem = _doc.createElement("ImageReaderData");
        _doc.appendChild( docElem );
        }

    QList <QString> _imageNames;
    QDomElement image( docElem.firstChildElement("image") );
    while( !image.isNull() )
        {
        if( QFile::exists( dirName + "/" + image.attribute("filename") ) && isAllowedName( image.attribute("filename") ) )
            {
            _imageNames << image.attribute("filename");
            _images << IImage( dirName, image, &_doc );
            image = image.nextSiblingElement("image");
            }
        else
            {
            QDomElement tmpRemove = image;
            image = image.nextSiblingElement("image");

            docElem.removeChild( tmpRemove );
            }
        }

    QDir dir( _directory );
    foreach( QFileInfo info, dir.entryInfoList( QDir::Files ) )
        {
        if( info.fileName() != "IRData.xml" && !_imageNames.contains( info.fileName() ) )
            {
            QDomElement imageElement = _doc.createElement("image");
            imageElement.setAttribute( "filename", info.fileName() );
            imageElement.setAttribute( "placeHolder", true );
            docElem.appendChild( imageElement );
            }
        }
    }

IDirectoryControl::~IDirectoryControl()
    {
    _images.empty();

    QFile file( _directory + "/" + "IRData.xml" );
    if( file.open( QIODevice::WriteOnly ) )
        {
        file.write( _doc.toString().toUtf8() );
        }
    }

QList <IImage> IDirectoryControl::files()
    {
    return _images;
    }
