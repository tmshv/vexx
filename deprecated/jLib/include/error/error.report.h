#ifndef JLIB_ERROR_REPORT_H
#define JLIB_ERROR_REPORT_H

#include "error/error.types.h"

BEGIN_JLIB_ERROR_NAMESPACE

class reportBuf;
class reportStream;
class report;

struct JLIB_EXPORT message
    {
    int type;
    string file;
    int line;
    string function;
    string message;
    bool extra;
    };

class JLIB_EXPORT reportBuf : public std::streambuf
    {
public:
    reportBuf( reportStream *p );
    int overflow( int c );
    string str();
    void clear();
private:
    reportStream *parent;
    string buffer;
    };

class JLIB_EXPORT reportStream : public std::ostream
    {
public:
    reportStream( report * );
    void submit();
    void clear();
private:
    reportBuf buffer;
    report *parent;
    };

typedef enum { RETURN = 1, ABORT = 2, REQUEUE = 4 } messageAction;

class JLIB_EXPORT reportHandler
    {
public:
    virtual int handle( message );
    };

struct handlerDef
    {
    int type;
    reportHandler *handler;
    };

class JLIB_EXPORT report
    {
public:
    report( report *p=0 );
    void setParent( report * );
    void setRoot();
    void submit( string );
    string formatMessage( string m );
    static reportStream &sendStatic( int, string, int, string, bool extra=1 );
    static void sendStringStatic( int, string, int, string, string );
    static void assignHandlerStatic( int, reportHandler & );
    static void assignHandlerStatic( int, reportHandler * );

    reportStream &send( int, string, int, string, bool extra=1 );
    void sendString( int, string, int, string, string );
    void assignHandler( int, reportHandler & );
    void assignHandler( int, reportHandler * );

private:
    vector <struct handlerDef> handlers;
    void resetCurrent();
    void process();
    message currentItem;
    vector <message> list;
    reportStream stream;
    reportStream &sendRoot( int, string, int, string, bool extra=1 );
    reportHandler defaultHandler;
    };

END_JLIB_ERROR_NAMESPACE


#define jReportCustom(a,b)   b.send( a, __FILE__, __LINE__, JLIB_CURRENT_FUNCTION )

#define jReport(a)           jLib::error::report::sendStatic( a, __FILE__, __LINE__, JLIB_CURRENT_FUNCTION )
#define jReportNeat(a)       jLib::error::report::sendStatic( a, __FILE__, __LINE__, JLIB_CURRENT_FUNCTION, 0 )
#define jReportString(a,b)   jLib::error::report::sendStringStatic( a, __FILE__, __LINE__, JLIB_CURRENT_FUNCTION, b )

#ifdef QT_NO_DEBUG
#   define jAssert(expr)
#   define jFixme           jReport(1)
#   define jDebug           jReport(250)
#   define jDebugNeat       jReportNeat(250)
#   define jFail( a )
#else
#   define jAssert(expr)                                            \
    ((expr)                                                         \
    ? (JLIB_ERROR_DO_NOTHING (0))                                   \
    : jReportString(2000,__STRING(Assertion Failed: expr)))
#   define jFixme       jReport(1)
#   define jDebug       jReport(250)
#   define jDebugNeat   jReportNeat(250)
#   define jFail( a )   jReportString( 2000, a )
#endif

#if 0
#   define jRenderDebug( a ) jReport( 250 )<<a<<endl;
#else
#   define jRenderDebug( a )
#endif


#define jWarning     jReport(500)
#define jError       jReport(1000)

using std::endl;

#ifdef QT_CORE_LIB

#include "QString"

inline std::ostream &operator<<( std::ostream &os, QString str )
    {
    os<<str.toStdString();
    return os;
    }

#endif

#endif
