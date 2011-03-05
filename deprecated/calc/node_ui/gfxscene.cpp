#include "gfxscene.h"
#include "node.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

gfxscene::gfxscene(QWidget* parent) : QGraphicsScene(parent)
{
    setSceneRect(-10000, -10000, 20000, 20000);
}
