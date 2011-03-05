#ifndef JLIB_FILE_SUBTAG_H
#define JLIB_FILE_SUBTAG_H

#include <file/file.types.h>

#ifdef JLIB_DEPRECATED

namespace jLib
    {
    namespace file
        {
        class subTag;
        class xmlAttrib;


        class JLIB_EXPORT subTagList
            {
        private:
            std::vector <subTag> list;

        public:
            subTagList( subTag base, std::string tagName );
            unsigned int size();
            subTag& operator[] ( const unsigned int _i );
            };

        class JLIB_EXPORT subTag
            {
            public:
            int startTag;
            int startContent;
            int endContent;
            std::string *data;

            subTag( );
            subTag( std::string &d );
            subTag( int z, int s, int e, std::string &d );

            //friend std::ostream& operator<<(std::ostream& output, subTag other );

            subTagList enumerate( std::string tagName );

            inline subTag findTag( std::string tag )
                { return findTag( tag, 1 ); }
            subTag findTag( int start, std::string tag, bool hS );
            subTag findTag( int start, std::string tag, xmlAttrib attribs, bool hS );
            subTag findTag( std::string tag, bool hS );
            subTag findTag( std::string tag, xmlAttrib attribs, int forceStart, bool hS );
            subTag findTag( std::string tag, xmlAttrib attribs, bool hS );

            int countNRE( std::string tag, std::string attrib, bool hS );
            int countNRE( std::string tag, std::string attrib, xmlAttrib attribs, bool hS );
            int countNRE( std::string tag, std::string attrib, bool hS, bool ordered );

            bool exists( );
            std::string getString( );
            jLib::math::vector4 getVector4( );
            jLib::math::triple getTriple( );
            float getFloat( );
            int getInt( );

            private:
            int getCloseTag( unsigned int start, std::string tag );
            int getCloseTagQuick( unsigned int start, std::string tag );
            };
        }
    }
#endif

#endif
