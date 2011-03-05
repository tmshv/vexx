//#include "APrivateWindow.h"
//#include "QApplication"
//#include "niceSplash.h"
#include "AApplication"
#include "APlugin"

static const char *recommendedAppSetup = "000000ff00000000fd00000001000000000000033b00000428fc0200000003fb0000000c0056006900650077006500720100000041000004280000005a00fffffffb0000000a004e006f00640065007301000000f4000000260000000000000000fc0000023f0000022a0000000000fffffffaffffffff0100000003fb0000001400440065006200750067002000540072006500650000000000ffffffff0000005900fffffffb00000006004c006f00670000000000ffffffff0000005400fffffffb0000001800520065006e0064006500720020004400650062007500670000000000000001e50000012d00ffffff000002ff0000042800000004000000040000000800000008fc0000000100000002000000010000001a004e006f006400650020004300720065006100740069006f006e0100000000ffffffff0000000000000000";

int main(int argc, char *argv[])
    {
#ifndef DONT_USE_AAPPLICATION
    AApplication a( "Alter", ":/icon.png", ":/calcSplash.png", argc, argv, AApplication::ForcedPlugins, AApplication::NoCreateMenu );

    a.sdk()->request( "baseFileTypes" );
    a.sdk()->request( "calc" );

#ifndef QT_NO_DEBUG
    a.sdk()->request( "debugBrowser" );
#endif

    return a.exec( "Nodes", QByteArray::fromHex( recommendedAppSetup ) );
#else
    QApplication a(argc, argv);

    QPixmap pixmap(":/splash.png");
    niceSplash splash(pixmap);
    splash.show();

    APrivateWindow w( "Alter" );

    w.show();
    splash.finish(&w);

    return a.exec();
#endif
    }
