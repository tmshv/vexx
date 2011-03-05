#ifndef JLIB_FILE_FILE_H
#define JLIB_FILE_FILE_H

#include <file/file.types.h>

#ifdef JLIB_DEPRECATED

#include <file/file.filename.h>

namespace jLib
    {
    namespace file
        {
        class xmlAttrib;
        class filename;

        void stringToChar( std::string a, std::vector <char> &out );
        void stringFromChar( std::string &a, std::string in, unsigned int &position );
        std::string makeTag( std::string tag );
        std::string makeTag( std::string tag, xmlAttrib attribs );
        std::string makeCloseTag( std::string tag );
        std::string makeTab( int tabs );
        //int fileExists( std::string filename );

        template <typename T> void toChar( T a, std::vector <char> &out )
            {
            char *conv = (char *)&a;

            for( unsigned int x=0; x<sizeof(T); x++ )
                {
                out.push_back( conv[x] );
                }
            }

        template <typename T> void fromChar( T &a, std::string in, unsigned int &pos )
            {
            for( unsigned int x=0; x<sizeof(T); x++ )
                {
                ((char *)&a)[x] = in[pos+x];
                }
            pos += sizeof(T);
            }

        template <class T> inline bool fromString(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
            {
            std::istringstream iss(s);
            return !(iss >> f >> t).fail();
            }

        template <class T> inline bool fromString(T& t, const std::string& s)
            {
            std::istringstream iss(s);
            return !(iss >> t).fail();
            }

        /*class JLIB_EXPORT filename
            {
        public:
            std::string fileFullName;
            std::string fileDirName;
            std::string fileBaseName;
            std::string fileExtName;
            std::string slashType;

            filename( );
            filename( std::string _f );
            void processName( std::string _f );
            };*/

        class JLIB_EXPORT file
            {
        private:
            bool _exists;
        public:
            filename name;
            std::string content;

            file();
            file( std::string fname );
            void replace( const std::string find, std::string replace );
            void load( );
            static bool exists( std::string _f );
            bool exists( ) { return _exists; }
            unsigned int length();
            };
        }
    }

#endif

#endif
