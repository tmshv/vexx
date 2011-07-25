#include <QApplication>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "gfxview.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    gfxview scene;

    scene.addNode();
    scene.addNode();

    QGraphicsView view( &scene );
    view.setRenderHints( QPainter::Antialiasing );
    view.show();

    return app.exec();


    return 1;
}
