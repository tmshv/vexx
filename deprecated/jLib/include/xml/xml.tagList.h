#ifndef JLIB_XML_TAGLIST_H
#define JLIB_XML_TAGLIST_H

#include "xml/xml.types.h"

BEGIN_JLIB_XML_NAMESPACE

//! Holds a set of tags, as a tag may contain
class JLIB_EXPORT tagList
    {
public:
    //! Iterate through the tags
    tag &operator[]( unsigned int );

    //! Iterate through the tags
    const tag &operator[]( unsigned int ) const;

    //! Find the number of tags in the list
    unsigned int size() const;
private:
    vector <tag *> _tags;
    friend class tag;
    };

END_JLIB_XML_NAMESPACE

#include <xml/xml.tag.h>

#endif
