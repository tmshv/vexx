#include <file/file.types.h>

#ifdef JLIB_DEPRECATED

#include <file/file.subTag.h>
#include <file/file.file.h>
#include <file/file.xmlAttrib.h>

namespace jLib
    {
    namespace file
        {
        subTagList::subTagList( subTag base, std::string tagName )
            {
            //cout<<"Find Fool"<<endl;
            subTag tag;

            tag = base.findTag( tagName, 1 );
            while( tag.exists() )
                {
                list.push_back( tag );
                tag = base.findTag( tag.endContent - base.startContent, tagName, 1 );
                }
            }

        unsigned int subTagList::size()
            {
            return list.size();
            }

        subTag& subTagList::operator[] ( const unsigned int _i )
            {
            return list[_i];
            }

        subTag::subTag( ) : startTag(STRFALSE), startContent(STRFALSE), endContent(STRFALSE), data(NULL)
            {;}

        subTag::subTag( std::string &d ) : startTag(STRFALSE), startContent(0), endContent(d.length()), data(&d)
            {;}

        subTag::subTag( int z, int s, int e, std::string &d ) : startTag(z), startContent(s), endContent(e), data(&d)
            {;}

        subTagList subTag::enumerate( std::string tagName )
            {
            return subTagList( *this, tagName );
            }

        subTag subTag::findTag( int start, std::string tag, bool hS )
            {
            start += startContent;

            unsigned int ret = data->find( "<"+tag, start );
            if( ret < data->length() && ( endContent == STRFALSE || ret < (unsigned int)endContent ) )
                {
                int close;

                if( hS == 0 )
                    {
                    close = getCloseTag( ret, tag );
                    }
            else
                    {
                    close = getCloseTagQuick( ret, tag );
                    }

                if( close < endContent )
                    {
                    return subTag( ret, data->find( ">", ret )+1, close, *data );
                    }
                else
                    {
                    return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
                    }
                }
            else
                {
                return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
                }
            }

        subTag subTag::findTag( int start, std::string tag, xmlAttrib attribs, bool hS )
            {
            int attribCheck = 0;

            start += startContent;

            while( start < (int)data->length() )
                {
                unsigned int tagStart = data->find( "<"+tag+" ", start );
                if( tagStart > data->length() )
                    {
                    tagStart = data->find( "<"+tag+">", start );
                    }
                int close;

                if( hS == 0 )
                    {
                    close = getCloseTag( tagStart, tag );
                    }
                else
                    {
                    close = getCloseTagQuick( tagStart, tag );
                    }
                subTag ret( tagStart, data->find( ">", tagStart )+1, close, *data );

                for( int x=0; x<attribs.size; x++ )
                    {
                    int name = data->find( attribs.name[x], tagStart );
                    int equals = data->find( "=", name+attribs.name[x].length() );
                    int value = data->find( attribs.value[x], equals+1 );

                    if( name < ret.startContent && equals < ret.startContent && value < ret.startContent && (*data)[value-1] == '"' && (*data)[value+attribs.value[x].length()] == '"' )
                        {
                        attribCheck++;
                        for( int y=name+attribs.name[x].length(); y<equals; y++ )
                            {
                            if( (*data)[y] != ' ' )
                                {
                                attribCheck=FALSE;
                                }
                            }
                        for( int y=equals+1; y<value-1; y++ )
                            {
                            if( (*data)[y] != ' ' )
                                {
                                attribCheck=FALSE;
                                }
                            }
                        }
                    }

                if( attribCheck == attribs.size )
                    {
                    return ret;
                    }
                else
                    {
                    attribCheck=0;
                    start++;
                    }
                }

            return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
            }

        subTag subTag::findTag( std::string tag, bool hS )
            {
            int start = startContent;

            unsigned int ret = data->find( "<"+tag+">", start );
            if( ret > data->length() )
                {
                ret = data->find( "<"+tag+" ", start );
                }

            if( ret < data->length() && ( endContent == STRFALSE || ret < (unsigned int)endContent ) )
                {
                //std::cout<<"x"<<std::endl;
                int close;
                if( hS == 0 )
                    {
                    close = getCloseTag( ret, tag );
                    }
                else
                    {
                    close = getCloseTagQuick( ret, tag );
                    }
                return subTag( ret, data->find( ">", ret )+1, close, *data );
                }
            else
                {
                return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
                }
            }

        subTag subTag::findTag( std::string tag, xmlAttrib attribs, int forceStart, bool hS )
            {
            //std::cout<<"Yeat again were checking the find tag function"<<std::endl;
            bool attribCheck = FALSE;
            int start = forceStart;

            while( start < endContent )
                {
                //std::cout<<start<<" "<<endContent<<std::endl;
                //std::cout<<"looking for <"+tag<<std::endl;
                int tagStart = data->find( "<"+tag+" ", start );
                int close;

                if( tagStart == STRFALSE )
                    {
                    return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
                    }

                if( hS == 0 )
                    {
                    close = getCloseTag( tagStart, tag );
                    }
                else
                    {
                    close = getCloseTagQuick( tagStart, tag );
                    }
                subTag ret( tagStart, data->find( ">", tagStart )+1, close, *data );

                if( attribs.check( ret ) )
                    {
                    return ret;
                    }
                else
                    {
                    attribCheck=0;
                    start=data->find( "<", tagStart+1 );
                    }
                }
            return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
            }

        subTag subTag::findTag( std::string tag, xmlAttrib attribs, bool hS )
            {
            //std::cout<<"Yeat again were checking the find tag function"<<std::endl;
            bool attribCheck = FALSE;
            int start = startContent;

            while( start < endContent )
                {
                //std::cout<<start<<" "<<endContent<<std::endl;
                //std::cout<<"looking for <"+tag<<std::endl;
                int tagStart = data->find( "<"+tag+" ", start );
                if( tagStart > (int)data->length() )
                    {
                    tagStart = data->find( "<"+tag+">", start );
                    }
                int close;

                if( tagStart == STRFALSE )
                    {
                    return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
                    }

                if( hS == 0 )
                    {
                    close = getCloseTag( tagStart, tag );
                    }
                else
                    {
                    close = getCloseTagQuick( tagStart, tag );
                    }
                subTag ret( tagStart, data->find( ">", tagStart )+1, close, *data );

                if( attribs.check( ret ) )
                    {
                    return ret;
                    }
                else
                    {
                    attribCheck=0;
                    start=data->find( "<", tagStart+1 );
                    }
                }
            return subTag( STRFALSE, STRFALSE, STRFALSE, *data );
            }

        int subTag::countNRE( std::string tag, std::string attrib, bool hS )
            {
            int num=0;
            subTag currentPt = findTag(tag, xmlAttrib(attrib,0),hS);
            while( currentPt.exists() )
                {
                #ifdef XML_DEBUG
                if( (num % 100) == 0 )
                    {
                    jDebug<<"Found "<<num<<" Elements so far";
                    }
                #endif
                currentPt = findTag(tag, xmlAttrib(attrib,++num),hS);
                }
            return num;
            }

        int subTag::countNRE( std::string tag, std::string attrib, bool hS, bool ordered )
            {
            int num=0;
            subTag currentPt = findTag(tag, xmlAttrib(attrib,0),hS);
            while( currentPt.exists() )
                {
                #ifdef XML_DEBUG
                if( (num % 100) == 0 )
                        {
                        jDebug<<"Found "<<num<<" Elements so far";
                        }
                #endif
                if( ordered )
                    {
                    currentPt = findTag(tag, xmlAttrib(attrib,++num),currentPt.endContent,hS);
                    }
                else
                    {
                    currentPt = findTag(tag, xmlAttrib(attrib,++num),hS);
                    }
                }
            return num;
            }

        int subTag::countNRE( std::string tag, std::string attrib, xmlAttrib attribs, bool hS )
            {
            //cout<<"Here the new countNRE function starts"<<endl;
            int num=0;
            xmlAttrib tempAttrib2=attribs;
            tempAttrib2.add(attrib,0);
            //cout<<"looking for num = "<<0<<endl;
            subTag currentPt = findTag( tag, tempAttrib2, hS );
            //int x=0;
            while( currentPt.exists() )
                {
                //x++;
                //cout<<currentPt<<endl;
                xmlAttrib tempAttrib=attribs;
                tempAttrib.add(attrib,++num);
                //cout<<"looking for "<<x<<" num = "<<num<<endl;
                currentPt = findTag( tag,  tempAttrib, hS );
                /*if( x > 2 )
                        {
                        exit(0);
                        }*/
                }

            return num;
            }

        bool subTag::exists( )
            {
            if( startTag != STRFALSE && startContent != STRFALSE && endContent != STRFALSE )
                {
                return TRUE;
                }
            else
                {
                return FALSE;
                }
            }

        std::string subTag::getString( )
            {
            std::string ret;
            for( int x=startContent; x<endContent; x++ )
                {
                ret += (*data)[x];
                }
            return ret;
            }

        float subTag::getFloat( )
            {
            std::string preret;
            float ret;
            for( int x=startContent; x<endContent; x++ )
                {
                preret += (*data)[x];
                }
            if( jLib::file::fromString <float> ( ret, preret ) )
                {
                return ret;
                }
            else
                {
                return 0;
                }
            }

        int subTag::getInt( )
            {
            std::string preret;
            int ret;
            for( int x=startContent; x<endContent; x++ )
                {
                preret += (*data)[x];
                }
            if( fromString <int> ( ret, preret ) )
                {
                return ret;
                }
            else
                {
                return 0;
                }
            }
        jLib::math::triple subTag::getTriple( )
            {
            jLib::math::triple ret;
            subTag temp = findTag( "x" );
            if( temp.exists() )
                {
                ret.x() = temp.getFloat();
                ret.y() = findTag( "y" ).getFloat();
                ret.z() = findTag( "z" ).getFloat();
                }
            temp = findTag( "r" );
            if( temp.exists() )
                {
                ret.x() = temp.getFloat();
                ret.y() = findTag( "g" ).getFloat();
                ret.z() = findTag( "b" ).getFloat();
                }
            temp = findTag( "u" );
            if( temp.exists() )
                {
                ret.x() = temp.getFloat();
                ret.y() = findTag( "v" ).getFloat();
                ret.z() = findTag( "w" ).getFloat();
                }
            return ret;
            }
        jLib::math::vector4 subTag::getVector4( )
            {
            jLib::math::vector4 ret;
            ret.x() = findTag( "x" ).getFloat();
            ret.y() = findTag( "y" ).getFloat();
            ret.z() = findTag( "z" ).getFloat();
            ret.w() = findTag( "w" ).getFloat();
            return ret;
            }

        int subTag::getCloseTag( unsigned int start, std::string tag )
            {
            unsigned int heirachy=1, pos = start+1;
            //cout<<"Start Close Tag"<<endl;
            while( heirachy != 0 && pos < data->length() )
                {
                //cout<<tag<<" "<<heirachy<<", "<<pos<<" < "<<data->length()<<endl;
                if( data->find( "</"+tag, pos ) == pos )
                    {
                    heirachy--;
                    }
                else if( data->find( "<"+tag, pos ) == pos )
                    {
                    heirachy++;
                    }
                pos=data->find( "<", pos+1 );
                }
            //cout<<"End Close Tag"<<endl;
            return ( data->rfind( "<", ( pos - 1 ) ) );
            }

        int subTag::getCloseTagQuick( unsigned int start, std::string tag )
            {
            return ( data->find( "</"+tag, start+1 ) );
            }
        }
    }

#endif
