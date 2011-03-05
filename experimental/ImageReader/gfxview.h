#ifndef GFXVIEW_H
#define GFXVIEW_H

#include "QGraphicsView"
#include "QGraphicsScene"

class IImageLibrary;
class ImageGraphicsItem;

class gfxView : public QGraphicsView
    {
public:
    gfxView( IImageLibrary * );
    ~gfxView( );
    QSize sizeHint() const;

protected:
    void drawBackground( QPainter *painter, const QRectF &rect );
    void resizeEvent( QResizeEvent *event );

private:
    QGraphicsScene _scene;
    IImageLibrary *_library;
    float _imageSize;
    int _imagesPerRow;
    float _actualImageSize;
    int _rows;
    QList <ImageGraphicsItem *> _images;
    };

#endif // GFXVIEW_H
