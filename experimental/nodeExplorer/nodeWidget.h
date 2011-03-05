#ifndef NODEWIDGET_H
#define NODEWIDGET_H

#include "QGraphicsItem"

class QGraphicsDropShadowEffect;

class nodeWidget : public QGraphicsItem
    {
public:
    nodeWidget();

    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
    QRectF boundingRect( ) const;
    QPainterPath shape() const;

protected:
    int minimumWidth( QFont ) const;
    QRectF titlebarRect( QFont ) const;
    QBrush titleGradient( QRectF ) const;
    QBrush contentsBrush( QRectF ) const;

private:
    QRectF _bounds;
    QPainterPath _titlePath;
    };

#endif // NODEWIDGET_H
