#include <file/file.types.h>

#ifdef JLIB_DEPRECATED

#include <file/file.file.h>
#include <file/file.xmlAttrib.h>

namespace jLib
    {
    namespace file
        {
        file::file() : _exists( FALSE )
            {;}

        file::file( std::string fname ) : _exists( FALSE ), name( fname )
            {
            load( );
            }

        /*filename::filename( )
            {;}

        filename::filename( std::string _f )
            {
            processName( _f );
            }

        void filename::processName( std::string _f )
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
            }*/

        void file::replace( const std::string find, std::string replace )
            {
            size_t j;
            for ( ; (j = content.find( find )) != std::string::npos ; )
                {
                content.replace( j, find.length(), replace );
                }
            }
        bool file::exists( std::string _f )
            {
            std::ifstream inFile( _f.c_str(), std::ifstream::in );
            if( inFile.is_open() )
                {
                inFile.close();
                return TRUE;
                }
            return FALSE;
            }
        void file::load( )
            {
            std::ifstream inFile( name.fileFullName.c_str(), std::ifstream::in );
            if( inFile.is_open() )
                {
                while( !inFile.eof() )
                    {
                    content += inFile.get();
                    }
                content[content.length()-1]='\0';
                inFile.close();
                _exists=TRUE;
                }
            else
                {
                _exists=FALSE;
                }
            }

        unsigned int file::length()
            {
            return content.length();
            }

        void stringToChar( std::string a, std::vector <char> &out )
            {
            toChar( a.length(), out );

            for( unsigned int x=0; x<a.length(); x++ )
                {
                toChar( a[x], out );
                }
            }

        void stringFromChar( std::string &a, std::string in, unsigned int &position )
            {
            std::string temp;
            std::string::size_type length;

            fromChar( length, in, position );
            for( std::string::size_type x=0; x<length; x++ )
                {
                char b;
                fromChar( b, in, position );
                temp += b;
                }
            a=temp;
            }

        std::string makeTag( std::string tag )
            {
            return "<"+tag+">";
            }

        std::string makeTag( std::string tag, xmlAttrib attribs )
            {
            std::string ret = "<"+tag;

            for( int x=0; x<attribs.size; x++ )
                {
                ret += " " + attribs.name[x]+"=\""+attribs.value[x]+"\"";
                }

            return ret+">";
            }

        std::string makeCloseTag( std::string tag )
            {
            return "</"+tag+">";
            }

        std::string makeTab( int tabs )
            {
            std::string ret;

            for( int x=0; x<tabs; x++ )
                {
                ret += '\t';
                }
            return ret;
            }

        std::ostream& operator<<( std::ostream& output, subTag other )
            {
            return output<<"["<<other.startTag<<" "<<other.startContent<<" "<<other.endContent<<"]";
            }
        }
    }

#endif
