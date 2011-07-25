#include "appBase.h"
#include "appViewport.h"
#include "propertiesDock.h"
#include "logDock.h"
#include "explorerDock.h"
#include "xmlFileType.h"
#include "../SDK/transformEntity.h"

#include "materialFront.h"
#include "latticeFront.h"
#include "groundFront.h"

int main(int argc, char *argv[])
    {
    QApplication a(argc, argv);

    appBase w( "PhysiCal Playground" );

    new appViewport( &w );

    w.addDockedItem( "Log", new logDock );
    w.addDockedItem( "Properties", new propertiesDock );
    w.addDockedItem( "Explorer", new explorerDock );

    w.addFileType( new xmlFileType );

    w.addCreator( materialFront::identifier(), materialFront::create );
    w.addCreator( latticeFront::identifier(), latticeFront::create );
    w.addCreator( groundFront::identifier(), groundFront::create );
    w.addCreator( transformEntity::identifier(), transformEntity::create );

    w.restoreState();

    w.show();
    return a.exec();
    }
