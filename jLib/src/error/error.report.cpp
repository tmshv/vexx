#include "error/error.report.h"
#include <iostream>
#include "QDebug"

BEGIN_JLIB_ERROR_NAMESPACE

report defaultReport;

reportBuf::reportBuf( reportStream *p ) : parent( p )
    {
    }

int reportBuf::overflow( int c )
    {
    if(c == std::char_traits<char>::eof())
        {
        return std::char_traits<char>::not_eof(c);
        }
    buffer += c;
    if( c == '\n' )
        {
        parent->submit();
        }
    return c;
    }

string reportBuf::str()
    {
    return buffer;
    }

void reportBuf::clear()
    {
    buffer = "";
    }

reportStream::reportStream( report *p ) : std::ostream( &buffer ), buffer( this ), parent( p )
    {
    }

void reportStream::submit()
    {
    parent->submit( parent->formatMessage( buffer.str() ) );
    }

void reportStream::clear()
    {
    buffer.clear();
    }

reportStream &report::sendStatic( int type, string file, int line, string function, bool extra )
    {
    return defaultReport.sendRoot( type, file, line, function, extra );
    }

void report::sendStringStatic( int type, string file, int line, string function, string message )
    {
    defaultReport.sendRoot( type, file, line, function );
    defaultReport.submit( message );
    }

reportStream &report::send( int type, string file, int line, string function, bool extra )
    {
    return sendRoot( type, file, line, function, extra );
    }

void report::sendString( int type, string file, int line, string function, string message )
    {
    sendRoot( type, file, line, function );
    submit( message );
    }

reportStream &report::sendRoot( int type, string file, int line, string function, bool extra )
    {
    currentItem.extra = extra;
    currentItem.type = type;
    currentItem.file = file;
    currentItem.line = line;
    currentItem.function = function;
    return stream;
    }

void report::resetCurrent()
    {
    }

report::report( report * ) : stream( this )
    {
    }

void report::setParent( report * )
    {
    }

void report::submit( string message )
    {
    currentItem.message = message;
    list.push_back( currentItem );
    resetCurrent();
    stream.clear();
    process();
    }

string report::formatMessage( string m )
    {
    size_t pos=0;
    while( ( pos = m.find( "\n", pos ) ) != string::npos )
        {
        m.erase( m.begin() + pos );
        }
    return m;
    }

void report::process()
    {
    for( unsigned int x=0; x<list.size(); x++ )
        {
        int action;
        bool handled = FALSE;
        for( unsigned int y=0; y<handlers.size(); y++ )
            {
            if( handlers[y].type == list[x].type || handlers[y].type == -1 )
                {
                action = handlers[y].handler->handle( list[x] );
                handled = TRUE;
                }
            }

        if( !handled )
            {
            action = defaultHandler.handle( list[x] );
            }

        if( ( action & jLib::error::REQUEUE ) == 0 )
            {
            list.erase( list.begin() + x );
            }

        if( ( action & jLib::error::ABORT ) == jLib::error::ABORT )
            {
#ifdef QT_NO_DEBUG
            exit( EXIT_FAILURE );
#else
            // segment for debug trace
            char *s = "h";
            *s = 'H';
            qCritical();
#endif
            }
        }
    }

void report::assignHandlerStatic( int level, reportHandler &handler )
    {
    assignHandlerStatic( level, &handler );
    }

void report::assignHandlerStatic( int type, reportHandler *handler )
    {
    struct handlerDef temp;
    temp.type = type;
    temp.handler = handler;
    return defaultReport.handlers.push_back( temp );
    }


void report::assignHandler( int level, reportHandler &handler )
    {
    assignHandler( level, &handler );
    }

void report::assignHandler( int type, reportHandler *handler )
    {
    struct handlerDef temp;
    temp.type = type;
    temp.handler = handler;
    return handlers.push_back( temp );
    }

int reportHandler::handle( message in )
    {
    if( in.extra )
        {
#ifndef WIN32
        std::cout<<"In file "<<in.file<<", in function "<<in.function<<" at line "<<in.line<<":"<<endl
                <<in.message<<endl;
#else
        qDebug()<<"In file "<<QString::fromStdString(in.file)<<", in function "<<QString::fromStdString(in.function)<<" at line "<<in.line<<":"<<endl
                <<QString::fromStdString(in.message);
#endif
        }
    if( in.type >= 1000 )
        {
        return jLib::error::ABORT;
        }
    else
        {
        return jLib::error::RETURN;
        }
    }

END_JLIB_ERROR_NAMESPACE
