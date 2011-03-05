#ifndef GRAPHICS_COLLADA_FILE_H
#define GRAPHICS_COLLADA_FILE_H

#include "graphics/collada/graphics.collada.types.h"
#include "graphics/collada/graphics.collada.base.h"

BEGIN_JLIB_GRAPHICS_COLLADA_NAMESPACE

class JLIB_EXPORT file : base
    {
public:
    file( string );
    file( jLib::file::ioFile );
    ~file();

    void setContent( string );

    unsigned int size( type );
    bool has( type );
    //base *get( type, unsigned int );
    template <typename T>vector <T> get( type t )
        {
        vector <T> ret;

        xml::tagList elements = root.findFirst( getFindString( t ) ).findAll( getSubFindString( t ) );

        for( unsigned int x=0; x<elements.size(); x++ )
            {
            ret.push_back( T( elements[x] ) );
            }
        return ret;
        }
    /*template <typename T>T *get( type t, unsigned int n )
        {
        jAssert( dynamic_cast <T *> ( get( t, n ) ) );
        return static_cast <T *> ( get( t, n ) );
        }*/
private:
    string getFindString( type );
    string getSubFindString( type );
    xml::tag root;
    string content;
    };

END_JLIB_GRAPHICS_COLLADA_NAMESPACE

#endif
