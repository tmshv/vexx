#ifndef NODEGRAPHICALDETAILS_H
#define NODEGRAPHICALDETAILS_H

#include <QGraphicsObject>
#include <nodeEntity.h>

class nodeGraphicalDetails : public QGraphicsObject
{
public:
    enum { Type = UserType + 2 };

    nodeGraphicalDetails(APlugin *, nodeEntity*);
    ~nodeGraphicalDetails();

    void setColour( QColor );
    void setDrawLabels( bool );
    QColor colour() const;

    QRectF getBRect();
    nodeEntity* getEntity();

    APlugin *app();

    QRectF thumbnailRect() const;

    QRectF currentTextRect() const;

private:
    virtual int type() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget);
    QRectF boundingRect() const;

    QRectF _thumbRect;
    QRectF bRect;
    QRectF oldBRect;
    nodeEntity* mNodeEntity;
    bool drawLabels;
    QColor mColour;
    APlugin *_app;
    QRectF _currentTextRect;
};

#endif // NODEGRAPHICALDETAILS_H
