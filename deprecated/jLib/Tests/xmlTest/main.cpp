#include "jLib.h"

using namespace jLib::xml;
using namespace jLib::file;
using namespace jLib;

int main(int argc, char *argv[])
    {
    tag someXML( "MASTERTAG" );

    someXML.attributes().add( attribute( "hello", 5 ) );

    someXML.add().type( tag::None );
    someXML[0].name( "HAHAHAHAAH" );

    someXML.add( tag( "hello" ) );
    someXML.add( tag( "hello2" ) );
    someXML.add( tag( "hello3" ) );

    someXML.findFirst( "hello" ).add( tag( "test" ) );
    someXML.findFirst( "hello" ).attributes().add( attribute( "boing", 5.5 ) );

    xmlFile xFile( someXML );

    jDebugNeat<<"##"<<xFile.toString()<<"##"<<endl;

    someXML = xFile.fromString( xFile.toString() );

    xmlFile xFile2( someXML );
    jDebugNeat<<"##"<<xFile2.toString()<<"##"<<endl;
    }
