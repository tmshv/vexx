#ifndef JLIB_XML_TAG_H
#define JLIB_XML_TAG_H

#include "xml/xml.types.h"
#include "xml/xml.tagList.h"
#include "xml/xml.attributeList.h"

BEGIN_JLIB_XML_NAMESPACE

//! Specifies an XML tag, either open, close or enclosed
class JLIB_EXPORT tag
    {
public:
    //! The type of the tag
    enum tagType { Open, Empty, None };

    jProperty( string, name )
    jProperty( tagType, type )

public:
    //! Construct a tag ( valid or invalid )
    tag( jLib::string="", bool invalid=FALSE );

    //! An invalid tag for comparison / returning
    static tag invalid;

    //! Whether or not the tag is valid
    bool exists( ) const;

    variant contents() const;

    //! A template to retreive the contents as a specific type
    template <typename T> vector <T> contentArrayAs( unsigned int count = 0 )
        {
        std::istringstream tempStream( contents().toString() );
        vector <T> ret;

        if( count != 0 )
            {
            ret.reserve( count );
            }

        while( !tempStream.eof() && (int)tempStream.tellg() != -1 )
            {
            T temp;
            tempStream>>temp;
            ret.push_back( temp );
            }


        return ret;
        }

    //! Find all of a certain type of tag
    tagList findAll( string );

    //! Find the first of a specific tag type
    tag &findFirst( string );

    //! Find the number of tags inside this one
    unsigned int size( ) const;

    //! Access specific tag
    tag &operator[]( unsigned int );

    //! Access specific tag
    const tag &operator[]( unsigned int ) const;

    //! Add a tag
    tag &add( const string &a="" );

    //! Insert a tag at a position
    tag &insert( unsigned int, const string &a="" );

    //! Add a string element
    tag &add( const tag & );

    //! Insert a string element at a position
    tag &insert( unsigned int, const tag & );

    //! Clear all tags
    void clear();

    attributeList &attributes();
    const attributeList &attributes() const;

private:
    bool _isInvalid;
    vector <tag> _subTags;
    attributeList _attributes;
    };

END_JLIB_XML_NAMESPACE

#if 0
//! Specifies an XML tag, either open, close or enclosed
class JLIB_EXPORT tag
    {
public:
    static tag &invalid();
    //! The type of the tag
    enum { OPEN, CLOSE, EMPTY, NONE } type;
    //! The name of the tag
    string tagName;

    //! Initiate from string
    tag( string *c, tag *p=0, size_t lookStart=0 );
    //! Copy a tag
    tag( const tag & );
    //! Retrieve an instance of a tag
    tag &operator=( const tag & );
    //! Destroy all my subTags and me
    ~tag( );
    //! Whether or not the tag is valid
    bool exists( );
    //! Retrieve the attribute list
    attributeList getAttrList();
    //! Iterate through all tags contained in this one
    tag &operator[]( unsigned int );
    //! Find all of a certain type of tag
    tagList findAll( string );
    //! Find the first of a specific tag type
    tag &findFirst( string );
    //! Find the number of tags inside this one
    unsigned int size( );
    //! A template to retreive the contents as a specific type
    template <typename T> T getContentsAs()
        {
        std::istringstream tempStream( getContentsAsString() );
        T tempReturn;
        tempStream>>tempReturn;
        return tempReturn;
        }
    //! A template to retreive the contents as a specific type
    template <typename T> vector <T> getContentArrayAs( unsigned int count = 0 )
        {
        std::istringstream tempStream( getContentsAsString() );
        vector <T> ret;

        if( count != 0 )
            {
            ret.reserve( count );
            }

        while( !tempStream.eof() )
            {
            T temp;
            tempStream>>temp;
            ret.push_back( temp );
            }

        return ret;
        }
    //! Retrieve the contents as bool
    //bool getB( );
    //! Retrieve the contents as bool
    //unsigned long long getULL( );
    //! Retrieve the contents as float
    //float getF( );
    //! Retrieve the contents as int
    //int getI( );
    //! Export XML TAG as a string, no content or partner tag
    string getXML( );
    //! Export the XML Tag with contents and close tag
    string getFullXML( );
protected:
    tag &getNonExistant();
private:
    //! Retrieve the contents as string
    string getContentsAsString( );
    //! Whether that class should free its own members?
    bool instance;
    //! A pointer to the static non existant tag which we return when a tag isnt found
    tag *nonExistant;
    //! The start of the start tag in the source - the '<' character in the string
    size_t startStart;
    //! The end of the start tag in the source - the '>' character in the string
    size_t startEnd;
    //! The start of the end tag in the source - the '<' character in the string - 0 for enclosed
    size_t endStart;
    //! The end of the end tag in the source - the '>' character in the string - 0 for enclosed
    size_t endEnd;
    //! The sub tags
    tagList subTags;
    //! The parent of this tag
    tag *parent;
    //! The data string that contains this tag
    string *data;
    };
#endif

#endif
