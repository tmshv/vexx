#ifndef JLIB_XML_XMLSTRING_H
#define JLIB_XML_XMLSTRING_H

#include "xml/xml.types.h"
#include "xml/xml.tag.h"

BEGIN_JLIB_XML_NAMESPACE

class JLIB_EXPORT xmlString
    {
public:
    enum fileFormat { NewLines=1, Tabs=2 };
    jProperty( int, format );

public:
    xmlString( const jLib::string & );
    xmlString( const xml::tag &, int );

    jLib::string toString() const;
    jLib::xml::tag fromString( );

private:
    size_t fromString( jLib::xml::tag &, const jLib::string &, int parentStartEnd=0 );
    jLib::string getSubTag( const jLib::xml::tag &, unsigned int=0 );
    jLib::string getOpener( jLib::string, const jLib::xml::attributeList &, bool close=FALSE );
    jLib::string getClose( jLib::string );
    jLib::string getAttribute( const jLib::xml::attribute & );
    jLib::string _data;
    /*xmlFile( string );
    xml::tag &operator()();
    bool exists();

    static string strip( string );
protected:
    string buffer;
    xml::tag contents;
    string filename;*/
    };

jLib::string strip( jLib::string );
jLib::string stripHeader( jLib::string );
jLib::string stripComments( jLib::string );

END_JLIB_XML_NAMESPACE

#endif
