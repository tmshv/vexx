#include <file/file.types.h>

#ifdef JLIB_DEPRECATED

#include <file/file.xmlAttrib.h>
#include <file/file.file.h>

namespace jLib
    {
    namespace file
        {
        xmlAttrib::xmlAttrib()
            {
            size=0;
            }

        xmlAttrib::xmlAttrib( subTag &a )
            {
            int x=a.startTag;
            while( x < a.startContent )
                {
                std::string n, v;
                int startn = a.data->find( " ", x );
                int endn = a.data->find( "=", startn );
                int startv = a.data->find( "\"", x );
                int endv = a.data->find( "\"", startv+1 );

                if( startn != -1 && endn != -1 && startv != -1 && endv != -1 && startn < a.startContent && endn < a.startContent && startv < a.startContent && endv < a.startContent )
                    {
                    for( int y=(startn+1); y<endn; y++ )
                        {
                        n += a.data->c_str()[y];
                        }
                    name.push_back( n );

                    for( int y=(startv+1); y<endv; y++ )
                        {
                        v += a.data->c_str()[y];
                        }
                    value.push_back( v );

                    x = endv+1;
                    }
                else
                    {
                    break;
                    }
                }
            }

        std::string xmlAttrib::getString( std::string v )
            {
            for( unsigned int x=0; x<name.size(); x++ )
                {
                if( name[x] == v )
                    {
                    return value[x];
                    }
                }
            return FALSE;
            }

        float xmlAttrib::getFloat( std::string v )
            {
            for( unsigned int x=0; x<name.size(); x++ )
                {
                if( name[x] == v )
                    {
                    float ret;
                    jLib::file::fromString( ret, value[x] );
                    return ret;
                    }
                }
            return 0.0;
            }

        int xmlAttrib::getInt( std::string v )
            {
            for( unsigned int x=0; x<name.size(); x++ )
                {
                if( name[x] == v )
                    {
                    int ret;
                    fromString( ret, value[x] );
                    return ret;
                    }
                }
            return 0;
            }

        xmlAttrib::xmlAttrib( std::string n, std::string v )
            {
            size=0;
            add( n, v );
            }

        xmlAttrib::xmlAttrib( std::string n, int v )
            {
            size=0;
            add( n, v );
            }

        xmlAttrib::xmlAttrib( std::string n, std::string v, xmlAttrib a )
            {
            name = a.name;
            value = a.value;
            size = a.size;

            add( n, v );
            }

        xmlAttrib::xmlAttrib( std::string n, int v, xmlAttrib a )
            {
            name = a.name;
            value = a.value;
            size = a.size;

            add( n, v );
            }

        void xmlAttrib::add( std::string n, std::string v )
            {
            name.push_back( n );
            value.push_back( v );
            size++;
            }

        void xmlAttrib::add( std::string n, int v )
            {
            std::string s;
            std::stringstream out;
            out << v;
            name.push_back(n);
            value.push_back(out.str());
            size++;
            }

        bool xmlAttrib::check( subTag& a )
            {
            int ret=0;

            for( int x=0; x<size; x++ )
                {
                int nameCheck = a.data->find( name[x]+"=\""+value[x]+"\"", a.startTag );
                if( nameCheck < ( a.startContent - 2 ) && nameCheck > a.startTag )
                    {
                    ret++;
                    }
                }
            if( ret == size )
                {
                //cout<<"return 1"<<endl;
                return TRUE;
                }
            else
                {
                //cout<<"return 0"<<endl;
                return FALSE;
                }
            }
        }
    }

#endif
