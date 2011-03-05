#include "file/file.dir.h"
#include <dirent.h>

BEGIN_JLIB_FILE_NAMESPACE

dir::dir( string _d )
    {
    DIR *handle;
    struct dirent *dName;

    if( ( handle = opendir( _d.c_str() ) ) == 0 )
        {
        exist = FALSE;
        }
    else
        {
        exist = TRUE;
        while ( ( dName = readdir( handle ) ) != 0 )
            {
            fileInfo temp;
            temp.name = string( dName->d_name );
            if( temp.name != "." && temp.name != ".." )
                {
#if defined (__MINGW32__)
                    if( opendir( (_d + "/" + temp.name).c_str() ) == 0 )
                        {
                        temp.type = fileInfo::FILE;
                        }
                    else
                        {
                        temp.type = fileInfo::DIRECTORY;
                        }
#else
                    if( dName->d_type == DT_UNKNOWN )
                        temp.type = fileInfo::UNKNOWN;
                    else if( dName->d_type == DT_REG )
                        temp.type = fileInfo::FILE;
                    else if( dName->d_type == DT_DIR )
                        temp.type = fileInfo::DIRECTORY;
                    else if( dName->d_type == DT_FIFO )
                        temp.type = fileInfo::FIFO;
                    else if( dName->d_type == DT_SOCK )
                        temp.type = fileInfo::SOCKET;
                    else if( dName->d_type == DT_CHR )
                        temp.type = fileInfo::CHARACTER;
                    else if( dName->d_type == DT_BLK )
                        temp.type = fileInfo::BLOCK;
#endif
                entries.push_back( temp );
                }
            }
        closedir( handle );
        }
    }

void dir::sort()
    {
    bool swapped=1;

    while( swapped )
        {
        swapped = FALSE;
        for( unsigned int x=0; x<( entries.size() - 1 ); x++ )
            {
            if( entries[x].name > entries[ x + 1 ].name )
                {
                fileInfo temp = entries[ x + 1 ];
                entries[ x + 1 ] = entries[x];
                entries[x] = temp;
                swapped = TRUE;
                }
            }
        }
    }

bool dir::exists( )
    {
    return exist;
    }

unsigned int dir::size()
    {
    return entries.size();
    }

fileInfo& dir::operator[] ( const unsigned int _i )
    {
    return entries[_i];
    }

END_JLIB_FILE_NAMESPACE
