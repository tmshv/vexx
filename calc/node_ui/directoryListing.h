#ifndef DIRECTORYLISTING_H
#define DIRECTORYLISTING_H

#include "QGraphicsObject"
#include "AEntityLocation"

class APlugin;
class gfxview;
class QGraphicsProxyWidget;

class directoryListing : public QGraphicsObject
    {
    Q_OBJECT
public:
    enum { Type = UserType + 6 };

    directoryListing( APlugin *, gfxview * );

    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );
    QRectF boundingRect() const;

    virtual int type() const;

signals:
    void changeDirectory( AEntityLocation );

private slots:
    void resize();
    void refreshPath();
    void changePath();

private:
    float _height;
    gfxview *_view;
    APlugin *_app;
    QGraphicsProxyWidget *_proxy;
    };

#endif // DIRECTORYLISTING_H
