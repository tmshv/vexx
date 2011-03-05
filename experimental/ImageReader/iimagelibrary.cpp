#include "iimagelibrary.h"
#include "QDir"
#include "QDebug"

IImageLibrary::IImageLibrary( QString str )
    {
    _base = str;
    index();
    }

IImageLibrary::~IImageLibrary( )
    {
    qDeleteAll( _dirs );
    }

void IImageLibrary::index()
    {
    QList <QString> todo;
    todo << _base;

    while( todo.size() )
        {
        QDir dir( todo.front() );
        foreach( QFileInfo info, dir.entryInfoList( QDir::Dirs|QDir::NoDotAndDotDot ) )
            {
            todo << info.absoluteFilePath();
            }

        _dirs << new IDirectoryControl( todo.front( ) );

        todo.removeFirst();
        }
    }

QList <IImage> IImageLibrary::files()
    {
    QList <IImage> ret;
    foreach( IDirectoryControl *dir, _dirs )
        {
        ret << dir->files();
        }
    return ret;
    }
