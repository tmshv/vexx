#ifndef JLIB_TESTENVIRONMENT_H
#define JLIB_TESTENVIRONMENT_H

#ifndef JLIB_LIBRARY

#include "jLib.h"

#define LOOPTESTTRUE( a, c, d ) ( a ) ? jDebugNeat<<"" : jError<<"Test "<<c<<" failed, internal data = "<<d<<endl;
#define LOOPTESTEQUAL( a, b, c, d ) ( a == b ) ? jDebugNeat<<"" : jError<<"Test "<<c<<" failed, internal data = "<<d<<endl;

#define TESTTRUE( a, c, d ) ( a ) ? jDebugNeat<<"Test "<<c<<" success"<<endl : jError<<"Test "<<c<<" failed, internal data = "<<d<<endl;
#define TESTEQUAL( a, b, c, d ) ( a == b ) ? jDebugNeat<<"Test "<<c<<" success"<<endl : jError<<"Test "<<c<<" failed, internal data = "<<d<<endl;

#ifndef NUMLOOPTESTS
#   define NUMLOOPTESTS 100000
#endif
#ifndef NUMLOOPPRINTS
#   define NUMLOOPPRINTS 10
#endif


#define BEGINTESTLOOP( TESTNAME, VAR ) { jLib::string testName = TESTNAME;                    \
    for( unsigned int VAR=0; VAR<NUMLOOPTESTS; ++VAR ) {                                      \
    if( ( VAR % ( NUMLOOPTESTS / NUMLOOPPRINTS ) ) == 0  )                                    \
        {                                                                                     \
        jDebugNeat<<testName<<"s "<<x/(NUMLOOPTESTS/100)<<"% comeplete"<<endl;                \
        }

#define ENDTESTLOOP }                                                                         \
    jDebugNeat<<testName<<"s complete successfully"<<endl; }

#endif

#endif
