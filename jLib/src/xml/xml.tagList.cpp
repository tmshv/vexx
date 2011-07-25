#include "xml/xml.tagList.h"

BEGIN_JLIB_XML_NAMESPACE

tag &tagList::operator[]( unsigned int x )
    {
    if( x < size() )
        {
        return *_tags.at(x);
        }
    return tag::invalid;
    }

const tag &tagList::operator[]( unsigned int x ) const
    {
    if( x < size() )
        {
        return *_tags.at(x);
        }
    return tag::invalid;
    }

unsigned int tagList::size() const
    {
    return _tags.size();
    }

END_JLIB_XML_NAMESPACE
