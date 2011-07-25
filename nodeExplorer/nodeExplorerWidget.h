#ifndef NODEEXPLORERWIDGET_H
#define NODEEXPLORERWIDGET_H

#include <QGraphicsView>

class QGraphicsScene;

class nodeExplorerWidget : public QGraphicsView
    {
public:
    nodeExplorerWidget( QWidget *parent = 0 );

protected:
    void resizeEvent( QResizeEvent * );
    void refreshBackgroundBrush();

private:
    QGraphicsScene *_scene;
    };

#endif // NODEEXPLORERWIDGET_H
