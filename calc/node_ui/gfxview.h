#ifndef GFXVIEW_H
#define GFXVIEW_H

#include <QGraphicsView>
#include <APlugin>
#include <QPixmap>
#include "nodepiemenu.h"

class gfxview : public QGraphicsView
{
    Q_OBJECT
public:
    gfxview(QWidget * = 0); // constructor
    ~gfxview( ); // dtor
    void setApp(APlugin *); // set the app base
    QTransform getTransform(); // returns the transform

protected:
    void resizeEvent(QResizeEvent *event);

    void mouseDoubleClickEvent( QMouseEvent * );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

    void keyReleaseEvent( QKeyEvent *event );
    void wheelEvent( QWheelEvent * event );

signals:
    void viewResized();
    void viewScaled();
    void viewMoved();

private slots:
    void create();
    void rebuildPieMenu();

private:
    APlugin* mApp;
    bool mScaling;
    bool mTranslating;
    bool mDragSelecting;


    void drawBackground ( QPainter * painter, const QRectF & rect );
    void drawForeground ( QPainter * painter, const QRectF & rect );
    QPixmap* bgMap;
    QTransform viewMatrix;

    QPointF mLastMousePos;
    QPointF mRootMenuPoint;
    QPointF mSelectRectStart;
    QPointF mSelectRectEnd;
    QtPieMenu* mRootMenu;
};

#endif // GFXVIEW_H
