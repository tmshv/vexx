#include "iimagelibrary.h"
#include "QApplication"
#include "gfxview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IImageLibrary lib("F:/Pictures/");

    gfxView w( &lib );
    w.show();
    return a.exec();
}
