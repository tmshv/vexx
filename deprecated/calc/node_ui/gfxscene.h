#ifndef GFXSCENE_H
#define GFXSCENE_H
#include <QGraphicsScene>
#include <QVector>

class gfxscene;

#include "node.h"

class gfxscene : public QGraphicsScene
    {
    Q_OBJECT
public:
    gfxscene(QWidget * = 0);

signals:
    void refreshScene();

private:
    QVector <node> nodes;
};

#endif // GFXVIEW_H
