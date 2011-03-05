#include "xml/xml.tag.h"

BEGIN_JLIB_XML_NAMESPACE

tag::tag( string n, bool i ) : _name( n ), _type( Open ), _isInvalid( i )
    {
    }

tag tag::invalid = tag( "INVALID", TRUE );

bool tag::exists( ) const
    {
    return _isInvalid == FALSE;
    }

variant tag::contents() const
    {
    if( size() == 1 && _subTags[0].type() == None )
        {
        return _subTags[0].name();
        }
    else if( type() == None && size() == 0 )
        {
        return name();
        }
    return "";
    }

tagList tag::findAll( string x )
    {
    tagList ret;

    for( unsigned int y=0; y<size(); y++ )
        {
        if( _subTags[y].name() == x )
            {
            ret._tags.push_back( &(_subTags[y]) );
            }
        }

    return ret;
    }

tag &tag::findFirst( string x )
    {
    jAssert( exists() );
    for( unsigned int y=0; y<size(); y++ )
        {
        if( _subTags[y].name() == x )
            {
            return _subTags[y];
            }
        }
    return invalid;
    }

unsigned int tag::size( ) const
    {
    return _subTags.size();
    }

tag &tag::operator[]( unsigned int x )
    {
    jAssert( x < _subTags.size() );
    return _subTags[x];
    }

const tag &tag::operator[]( unsigned int x ) const
    {
    jAssert( x < _subTags.size() );
    return _subTags[x];
    }

tag &tag::add( const string &in )
    {
    _subTags.push_back( in );
    _subTags.back().type( None );
    return _subTags.back();
    }

tag &tag::insert( unsigned int x, const string &in )
    {
    if( x > size() )
        {
        x = size();
        }
    _subTags.insert( _subTags.begin() + x, in );
    _subTags[x].type( None );
    return _subTags[x];
    }

tag &tag::add( const tag &in )
    {
    _subTags.push_back( in );
    return _subTags.back();
    }

tag &tag::insert( unsigned int x, const tag &in )
    {
    if( x > size() )
        {
        x = size();
        }
    _subTags.insert( _subTags.begin() + x, in );
    return _subTags[x];
    }

void tag::clear()
    {
    _subTags.clear();
    }

attributeList &tag::attributes()
    {
    return _attributes;
    }

const attributeList &tag::attributes() const
    {
    return _attributes;
    }

END_JLIB_XML_NAMESPACE

#if 0
tag::tag( string *c, tag *p, size_t lookStart ) : instance( FALSE ), nonExistant( 0 ), startStart( 0 ), startEnd( 0 ), endStart( 0 ), endEnd( 0 )
    {
    data = c;

    if( data != 0 )
        {
        jAssert( c->length() > 0 );
        parent = p;
        size_t parentStartEnd;

        if( lookStart != 0 )
            {
            parentStartEnd = lookStart;
            }
        else if( parent == 0 )
            {
            parentStartEnd = 0;
            }
        else
            {
            parentStartEnd = parent->startEnd;
            }

        startStart = data->find( "<", parentStartEnd );
        startEnd = data->find( ">", startStart );

        if( startStart == string::npos || startEnd == string::npos )
            {
            jDebugNeat<<"Attempting to parse tag inside tag "<<parent->tagName<<",  starting at character "<<parent->startEnd<<endl;
            jFail( "Could not find the start or end of a tag" );
            }

        if( (*data)[startEnd-1] == '/' )
            {
            type = EMPTY;
            endStart = 0;
            endEnd = 0;

            for( unsigned int x=(startStart+1); x<startEnd; x++ )
                {
                tagName += (*data)[x];
                }

            size_t temp;
            if( ( temp = tagName.find( " " ) ) < startEnd )
                {
                tagName.erase( temp, tagName.length() - temp );
                }
            else
                {
                tagName.erase( tagName.length() - 1 );
                }
            //std::cout<<"E"<<tagName<<std::endl;
            return;
            }
        else if( (*data)[startStart+1] == '/' )
            {
            type = CLOSE;
            endStart = 0;
            endEnd = 0;

            for( unsigned int x=(startStart+1); x<startEnd; x++ )
                {
                tagName += (*data)[x];
                }
            //std::cout<<"S"<<tagName<<std::endl;

            return;
            }
        else
            {
            type = OPEN;

            size_t temp = data->find( " ", startStart );

            if( temp > startEnd )
                {
                temp = startEnd;
                }

            for( unsigned int x=(startStart+1); x<temp; x++ )
                {
                tagName += (*data)[x];
                }
            //std::cout<<"G"<<tagName<<std::endl;
            }

        size_t currentStart = startEnd+1;
        while( TRUE )
            {
            //std::cout<<"Spout thing "<<this<<" - "<<currentStart<<std::endl;
            tag *temp = new tag( data , this, currentStart );
            if( temp->type == CLOSE )
                {
                if( "/"+tagName == temp->tagName )
                    {
                    endStart = temp->startStart;
                    endEnd = temp->startEnd;
                    delete temp;
                    break;
                    }
                else
                    {
                    jError<<"MalFormed XML at character "<<startStart<<" approximately, found a close tag - "<<temp->tagName<<" when I was looking for a close for "<<"/"+tagName<<endl;
                    }
                }
            else if( temp->type == OPEN )
                {
                subTags.push_back( temp );
                currentStart = temp->endEnd+1;
                }
            if( temp->type == EMPTY )
                {
                subTags.push_back( temp );
                currentStart = temp->startEnd+1;
                }

            //std::cout<<"Set currentStart to: "<<currentStart<<std::endl;
            }
        }
    }

tag::tag( const tag &in ) : type( in.type ), tagName( in.tagName ), instance( TRUE ), nonExistant( in.nonExistant ), startStart( in.startStart ),
        startEnd( in.startEnd ), endStart( in.endStart ), endEnd( in.endEnd ), subTags( in.subTags ), parent( in.parent ), data( in.data )
    {
    }

tag &tag::operator=( const tag &in )
    {
    jAssert( instance == TRUE );
    type = in.type;
    tagName = in.tagName;
    instance = TRUE;
    nonExistant = in.nonExistant;
    startStart = in.startStart;
    startEnd = in.startEnd;
    endStart = in.endStart;
    endEnd = in.endEnd;
    subTags = in.subTags;
    parent = in.parent;
    data = in.data;
    return *this;
    }

bool tag::exists( )
    {
    return data != 0;
    }

attributeList tag::getAttrList()
    {
    string temp;
    attributeList ret;

    for( unsigned int x=startStart+tagName.length()+1; x<startEnd; x++ )
        {
        temp += (*data)[x];
        }
    temp += " ";

    size_t equals = 0;
    while( TRUE )
        {
        attribute tempAttribute;
        equals = temp.find( "=", equals + 1 );
        if( equals == string::npos )
            {
            break;
            }
        size_t identifierStart = temp.rfind( " ", equals-1 );
        size_t valueEnd = temp.find( "\"", equals+2 );
        for( unsigned int x=( identifierStart + 1 ); x<equals; x++ )
            {
            tempAttribute.identifier += temp[x];
            }
        for( unsigned int x=( equals + 2 ); x<( valueEnd ); x++ )
            {
            tempAttribute.value += temp[x];
            }

        ret.push_back( tempAttribute );
        }
    return ret;
    }

unsigned int tag::size( )
    {
    return subTags.size();
    }

tag &tag::operator[]( unsigned int x )
    {
    return subTags[x];
    }
tagList tag::findAll( string x )
    {
    tagList ret;

    for( unsigned int y=0; y<size(); y++ )
        {
        if( subTags[y].tagName == x )
            {
            ret.push_back( subTags(y) );
            }
        }

    return ret;
    }

tag &tag::findFirst( string x )
    {
    for( unsigned int y=0; y<size(); y++ )
        {
        if( subTags[y].tagName == x )
            {
            return subTags[y];
            }
        }
    return getNonExistant();
    }

tag &tag::getNonExistant()
    {
    if( nonExistant == 0 )
        {
        nonExistant = new tag( 0 );
        }
    return *nonExistant;
    }

string tag::getContentsAsString( )
    {
    if( data != 0 )
        {
        string ret;
        for( unsigned int x=( startEnd + 1 ); x<endStart; x++ )
            {
            ret += (*data)[x];
            }
        return ret;
        }
    return "";
    }

/*bool tag::getB( )
    {
    bool ret;
    std::istringstream temp( get() );
    temp>>ret;
    return ret;
    }
unsigned long long tag::getULL( )
    {
    unsigned long long ret;
    std::istringstream temp( get() );
    temp>>ret;
    return ret;
    }
float tag::getF( )
    {
    float ret;
    std::istringstream temp( get() );
    temp>>ret;
    return ret;
    }
int tag::getI( )
    {
    int ret;
    std::istringstream temp( get() );
    temp>>ret;
    return ret;
    }*/

tag::~tag( )
    {
    if( !instance )
        {
        if( nonExistant != 0 )
            {
            delete nonExistant;
            }
        for( unsigned int x=0; x<subTags.size(); x++ )
            {
            delete subTags( x );
            }
        }
    }
#endif

