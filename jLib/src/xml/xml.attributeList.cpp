#include "xml/xml.attributeList.h"

BEGIN_JLIB_XML_NAMESPACE

variant attributeList::find( string in )
    {
    for( unsigned int x=0; x<_attributes.size(); x++ )
        {
        if( in == _attributes.at(x).identifier() )
            {
            return _attributes.at(x).value();
            }
        }
    return variant();
    }

unsigned int attributeList::size() const
    {
    return _attributes.size();
    }

attribute &attributeList::operator[]( unsigned int x )
    {
    jAssert( x < _attributes.size() );
    return _attributes[x];
    }

const attribute &attributeList::operator[]( unsigned int x ) const
    {
    jAssert( x < _attributes.size() );
    return _attributes[x];
    }

void attributeList::add( const attribute &in )
    {
    _attributes.push_back( in );
    }

void attributeList::insert( unsigned int x, const attribute &in )
    {
    jAssert( x < size() );
    _attributes.insert( _attributes.begin() + x, in );
    }

void attributeList::clear()
    {
    _attributes.clear();
    }

END_JLIB_XML_NAMESPACE
