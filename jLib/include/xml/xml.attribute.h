#ifndef JLIB_XML_ATTRIBUTE_H
#define JLIB_XML_ATTRIBUTE_H

#include "xml/xml.types.h"

BEGIN_JLIB_XML_NAMESPACE

//! Holds a single XML Attribute
class JLIB_EXPORT attribute
    {
    jProperty( string, identifier )
    jProperty( variant, value )
public:

    //! Initiate the attribute as blank
    attribute( );

    //! Initiate the attribute from two strings
    attribute( string, variant );

    //! Compare attributes
    bool operator==( const attribute & );
    };

END_JLIB_XML_NAMESPACE

#endif
