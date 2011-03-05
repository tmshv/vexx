#include "widget.h"
#include "QPainter"
#include "QDebug"
#include "math.h"
#include "QKeyEvent"

inline float f( float x )
    {
    return 50 * sin( x * x * 0.01 );
    }

inline float f1( float x )
    {
    return x * cos( x * x * 0.01 );
    }

inline float f2( float x )
    {
    return cos( x * x * 0.01 ) - 0.02 * x * x * sin( x * x * 0.01 );
    }

Widget::Widget(QWidget *parent) : QWidget(parent)
    {
    setFocusPolicy( Qt::StrongFocus );
    h = 1;
    }

Widget::~Widget()
    {
    }

struct particle
    {
    float p, v;

    float a()
        {
        return -p;
        }
    };

void Widget::paintEvent(QPaintEvent *)
    {
    QPainter pai( this );
    pai.drawText( 0, 0, QString::number( h ) );
    pai.setRenderHint( QPainter::Antialiasing );
    pai.translate( 0, (height()/2) );
    pai.scale( 300, -2 );

    pai.drawLine( QPointF( 20, 0 ), QPointF( -20, 0 ) );
    pai.drawLine( QPointF( 0, 20 ), QPointF( -0, -20 ) );

    float initialPos = 80;


    particle p;
    p.v = 0;
    p.p = initialPos;
    pai.setPen( Qt::blue );
    for( float x=0; x<(width()/2); x+=h )
        {
        float lastP = p.p;
        p.v += p.a() * h;
        p.p += p.v * h + h * h * p.a();
        pai.drawLine( QPointF( x+h, p.p ), QPointF( x, lastP ) );
        }

    p.v = 0;
    p.p = initialPos;
    pai.setPen( Qt::green );
    for( float x=0; x<(width()/2); x+=h )
        {
        float lastP = p.p;
        p.v += p.a() * h;
        p.p += p.v / ( ( 1 / h ) - p.a() );
        pai.drawLine( QPointF( x+h, p.p ), QPointF( x, lastP ) );
        }
    }

void Widget::keyPressEvent(QKeyEvent *event)
    {
    if( event->key() == Qt::Key_Up )
        {
        h += 0.01;
        }
    else if( event->key() == Qt::Key_Down )
        {
        h -= 0.01;
        }
    if( h < 0.01 )
        {
        h = 0.01;
        }
    update();
    }

#if 0
void Widget::paintEvent(QPaintEvent *)
    {
    float lastPos;

    QPainter pai( this );
    pai.drawText( 0, 0, QString::number( h ) );
    pai.setRenderHint( QPainter::Antialiasing );
    pai.translate( 0, (height()/2) );
    pai.scale( 300, -2 );

    pai.drawLine( QPointF( 20, 0 ), QPointF( -20, 0 ) );
    pai.drawLine( QPointF( 0, 20 ), QPointF( -0, -20 ) );

    float initialPos = 200;
    float k = 8.2;

    lastPos = initialPos;
    pai.setPen( Qt::blue );
    for( float x=0; x<(width()/2); x+=h )
        {
        float tempPos = lastPos + h * -k * lastPos;
        pai.drawLine( QPointF( x+h, tempPos ), QPointF( x, lastPos ) );
        lastPos = tempPos;
        }

    lastPos = initialPos;
    pai.setPen( Qt::green );
    for( float x=0; x<(width()/2); x+=h )
        {
        float tempPos = lastPos + ( -h / ( 1 + k * h ) ) * k * lastPos;
        pai.drawLine( QPointF( x+h, tempPos ), QPointF( x, lastPos ) );
        lastPos = tempPos;
        }
    }
#endif
