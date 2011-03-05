#ifndef JLIB_FILE_XMLATTRIB_H
#define JLIB_FILE_XMLATTRIB_H

#include <file/file.types.h>

#ifdef JLIB_DEPRECATED

#include <file/file.subTag.h>

namespace jLib
    {
    namespace file
        {
        class JLIB_EXPORT xmlAttrib
            {
        public:
            std::vector <std::string> name;
            std::vector <std::string> value;
            int size;

            xmlAttrib();
            xmlAttrib( subTag &a );
            xmlAttrib( std::string n, std::string v );
            xmlAttrib( std::string n, int v );
            xmlAttrib( std::string n, std::string v, xmlAttrib a );
            xmlAttrib( std::string n, int v, xmlAttrib a );

            void add( std::string n, std::string v );
            void add( std::string n, int v );
            bool check( subTag &a );

            std::string getString( std::string v );
            float getFloat( std::string v );
            int getInt( std::string v );
            };
        }
    }
	
#endif

#endif
