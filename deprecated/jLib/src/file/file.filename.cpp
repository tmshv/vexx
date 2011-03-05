#include "file/file.filename.h"

BEGIN_JLIB_FILE_NAMESPACE

filename::filename( )
    {;}

filename::filename( string _f )
    {
    processName( _f );
    }

void filename::processName( string _f )
    {
    fileFullName = _f;
    unsigned int dot, slash;

#if defined (_WIN32)
    slashType = '\\';
#else
    slashType = '/';
#endif

    //cout<<filename<<" "<<filename.length()<<endl;

    dot = fileFullName.length();
    slash = fileFullName.length();

    //std::cout<<slash<<std::endl;

    for( unsigned int x=(fileFullName.length()-1); x<fileFullName.length(); x-- )
        {
        if( fileFullName[x] == '.' && dot == fileFullName.length() )
            {
            //cout<<"Dot found at "<<x<<endl;
            dot = x;
            }
        else if( fileFullName[x] == '\\' && slash == fileFullName.length() )
            {
            slashType = '\\';
            slash = x;
            }
        else if( fileFullName[x] == '/' && slash == fileFullName.length() )
            {
            slashType = '/';
            slash = x;
            }
        if( dot != fileFullName.length() && slash != fileFullName.length() )
            {
            break;
            }
        }

    //cout<<slash<<" "<<dot<<endl;

    if( slash == fileFullName.length() )
        {
        //cout<<"Slash Reset"<<endl;
        slash = 0;
        slash--;
        }

    //cout<<( slash + 1 )<<" "<<dot<<endl;

    if( dot < ( slash + 1 ) )
        {
        //cout<<"Dot Reset"<<endl;
        dot = fileFullName.length();
        }

    for( unsigned int x=0; x<(slash+1); x++ )
        {
        fileDirName += fileFullName[x];
        }

    for( unsigned int x=(slash+1); x<dot; x++ )
        {
        fileBaseName += fileFullName[x];
        }

    for( unsigned int x=(dot+1); x<fileFullName.length(); x++ )
        {
        fileExtName += fileFullName[x];
        }
    //std::cout<<"#"<<fileDirName<<"#"<<fileBaseName<<"#"<<fileExtName<<"#"<<std::endl;
    }

END_JLIB_FILE_NAMESPACE
