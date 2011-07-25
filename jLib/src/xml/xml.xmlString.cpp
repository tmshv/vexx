#include "xml/xml.xmlString.h"

BEGIN_JLIB_XML_NAMESPACE

xmlString::xmlString( const jLib::string &in ) : _data( in )
    {
    }
xmlString::xmlString( const tag &in, int f ) : _format( f ), _data( getSubTag( in ) )
    {
    }

string xmlString::toString() const
    {
    return _data;
    }

string xmlString::getSubTag( const tag &in, unsigned int tabNum )
    {
    string ret;

    string tabs;
    if( (_format&Tabs) != FALSE )
        {
        for( unsigned int x=0; x<tabNum; x++ )
            {
            tabs += "\t";
            }
        }

    if( in.type() == tag::None )
        {
        ret += in.contents().toString();

        /*if( (_format&NewLines) != FALSE )
            {
            ret += "\n";
            }*/
        }
    else
        {
        if( in.type() == tag::Open && in.name() != "" )
            {
            ret += tabs + getOpener( in.name(), in.attributes() );
            }
        else if( in.type() == tag::Empty && in.name() != "" )
            {
            ret += tabs + getOpener( in.name(), in.attributes(), TRUE );
            }

        if( (_format&NewLines) != FALSE && ( in.size() > 1 || ( in.size() > 0 && in[0].type() != tag::None ) ) )
            {
            ret += "\n";
            }

        for( unsigned int x=0; x<in.size(); x++ )
            {
            ret += getSubTag( in[x], tabNum + 1 );
            }

        if( in.type() == tag::Open && in.name() != "" )
            {
            if( in.size() > 1 || ( in.size() > 0 && in[0].type() != tag::None ) )
                {
                ret += tabs;
                }
            ret += getClose( in.name() );
            }

        if( (_format&NewLines) != FALSE )
            {
            ret += "\n";
            }
        }

    return ret;
    }

string xmlString::getOpener( string name, const attributeList &attrs, bool close )
    {
    string ret = "<" + name;

    for( unsigned int x=0; x<attrs.size(); x++ )
        {
        ret += " " + getAttribute( attrs[x] );
        }

    if( close )
        {
        ret += " /";
        }

    return ret + ">";
    }

string xmlString::getClose( string in )
    {
    return "</" + in + ">";
    }

string xmlString::getAttribute( const attribute &attr )
    {
    return attr.identifier() + "=\"" + attr.value().toString() + "\"";
    }

tag xmlString::fromString( )
    {
    tag ret;
    fromString( ret, _data, 0 );
    return ret;
    }

size_t xmlString::fromString( tag &parent, const string &data, int parentStartEnd )
    {
    if( data != "" )
        {
        tag &added = parent.add( );

        size_t startStart = data.find( "<", parentStartEnd );
        size_t startEnd = data.find( ">", startStart );

        if( startStart == string::npos || startEnd == string::npos )
            {
            jDebugNeat<<"Attempting to parse tag inside tag "<<parent.name()<<",  starting at character "<<parent.name()<<endl;
            jDebugNeat << "Could not find the start or end of a tag" << endl;
            return data.size()-1;
            }

        if( data[startStart+1] != '/' )
            {
            size_t temp = data.find( " ", startStart );

            if( temp > startEnd )
                {
                temp = startEnd;
                }

            string tagName;
            for( unsigned int x=(startStart+1); x<temp; x++ )
                {
                tagName += data[x];
                }
            added.name( tagName );

            // attribute parsing
                {
                string temp;
                attributeList &attrs = added.attributes();

                for( unsigned int x=startStart+tagName.length()+1; x<startEnd; x++ )
                    {
                    temp += data[x];
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
                    string ident;
                    for( unsigned int x=( identifierStart + 1 ); x<equals; x++ )
                        {
                        ident += temp[x];
                        }
                    tempAttribute.identifier( ident );
                    string val;
                    for( unsigned int x=( equals + 2 ); x<( valueEnd ); x++ )
                        {
                        val += temp[x];
                        }
                    tempAttribute.value( val );

                    attrs.add( tempAttribute );
                    }
                }

            if( data[startEnd-1] == '/' )
                {
                added.type( tag::Empty );
                return startEnd;
                }
            added.type( tag::Open );
            }

        size_t oldTag = startStart;
        size_t currentTag = data.find( "<", startEnd+1 );
        while( currentTag != string::npos )
            {
            size_t oldEnd = data.find( ">", oldTag );
            if( currentTag > oldEnd + 1 )
                {
                string stuff;
                size_t begin = 0, end = 0;
                for( size_t x=oldEnd+1; x<currentTag; x++ )
                    {
                    if( !std::isspace( data[x] ) )
                        {
                        if( begin == 0 )
                            {
                            begin = x;
                            }
                        end = x;
                        }
                    }
                if( end != 0 )
                    {
                    for( size_t x=begin; x<=end; x++ )
                        {
                        stuff += data[x];
                        }
                    }
                added.add( stuff );
                }
            if( data[currentTag+1] == '/' )
                {
                size_t endEnd = data.find( '>', currentTag );
                string endName;
                for( size_t x=currentTag+2; x<endEnd; x++ )
                    {
                    endName += data[x];
                    }
                if( endName == added.name() )
                    {
                    return endEnd;
                    }
                }
            else
                {
                currentTag = fromString( added, data, currentTag-1 );
                }
            oldTag = currentTag;
            currentTag = data.find( "<", currentTag+1 );
            }
        }
    return string::npos;
    }


string strip( string x )
    {
    return stripComments( stripHeader( x ) );
    }

string stripComments( string x )
    {
    size_t begin = 0;
    while( ( begin = x.find( "<!--", begin ) ) != string::npos )
        {
        size_t end = x.find( "-->", begin );
        x.erase( begin, end - begin + 3 );
        begin += 4;
        }
    return x;
    }

string stripHeader( string x )
    {
    size_t b = x.find( "<?" );
    if( b != string::npos )
        {
        size_t a = x.find( ">", b );
        if( a != string::npos )
            {
            x.erase( b, 2 + a );
            }
        }

    return x;
    }

#if 0

xmlFile::xmlFile( string _n ) : buffer( stripComments( stripHeader( ioFile::get( _n ) ) ) ), contents( &( buffer ) ), filename( _n )
    {
    } &xmlFile::operator()()
    {
    return contents;
    }

bool xmlFile::exists()
    {
    return ioFile::exists( filename );
    }

#endif

END_JLIB_XML_NAMESPACE
