#ifndef ADARKSTYLE_H
#define ADARKSTYLE_H

#include "QCleanlooksStyle"

class ADarkStyle : public QCleanlooksStyle
    {
public:
    ADarkStyle();

    void polish( QApplication *app );
    void polish( QPalette &palette );
    void drawControl( ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget=0 ) const;
    void drawComplexControl( ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget=0 ) const;
    void drawPrimitive( PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget=0 ) const;
    QSize sizeFromContents( ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget=0 ) const;
    int pixelMetric( PixelMetric metric, const QStyleOption *option=0, const QWidget *widget=0 ) const;

private:
    QPixmap menuLoop;
    QPixmap radioDown;
    QPixmap radioUp;
    QPixmap radioOver;
    QPixmap checkUp;
    QPixmap checkDown;
    QPixmap checkOver;
    QPixmap checkTri;
    };

#endif // ADARKSTYLE_H
