#ifndef JLIB_XML_ATTRIBUTELIST_H
#define JLIB_XML_ATTRIBUTELIST_H

#include "xml/xml.types.h"
#include "xml/xml.attribute.h"

BEGIN_JLIB_XML_NAMESPACE

//! Holds a set of attributes, as an XML tag contains
class JLIB_EXPORT attributeList
    {
public:
    //! Find a identifier
    variant find( string );

    //! Number of attributes in list
    unsigned int size() const;

    //! Access specific attribute
    attribute &operator[]( unsigned int );

    //! Access specific attribute
    const attribute &operator[]( unsigned int ) const;

    //! Add an attribute
    void add( const attribute & );

    //! Insert an attribute at a point
    void insert( unsigned int, const attribute & );

    //! Clear all attributes
    void clear();

private:
    //! Attributes
    vector <attribute> _attributes;
    };

END_JLIB_XML_NAMESPACE

#endif
