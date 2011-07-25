#include "xml/xml.attribute.h"

BEGIN_JLIB_XML_NAMESPACE

attribute::attribute( )
    {
    }

attribute::attribute( string n, variant v ) : _identifier( n ), _value( v )
    {
    }

END_JLIB_XML_NAMESPACE
