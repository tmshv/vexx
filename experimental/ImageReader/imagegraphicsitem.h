#ifndef IMAGEGRAPHICSITEM_H
#define IMAGEGRAPHICSITEM_H

#include "QGraphicsObject"
#include "iimage.h"

class ImageGraphicsItem : public QGraphicsObject
    {
    Q_OBJECT
public:
    ImageGraphicsItem( IImage );

    void setSize( float size );

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );

private slots:
    void thumbnailReady();

private:
    IImage _img;
    float _size;
    QRectF _shapeRect;
    static QImage _invalid;
    static QImage _shadow;
    };

#endif // IMAGEGRAPHICSITEM_H
