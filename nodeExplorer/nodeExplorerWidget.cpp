#include "nodeExplorerWidget.h"

#include "QApplication"
#include "math.h"

#include "QDialog"
#include "nodeWidget.h"
#include "QGraphicsProxyWidget"

#include "QGraphicsDropShadowEffect"

#include "QGLWidget"

nodeExplorerWidget::nodeExplorerWidget( QWidget *parent ) : QGraphicsView( parent ), _scene( new QGraphicsScene( this ) )
    {
    setRenderHints( QPainter::Antialiasing );
    setDragMode( QGraphicsView::RubberBandDrag );
    setBackgroundBrush( QApplication::palette().color( QPalette::Mid ) );

    setScene( _scene );
    //refreshBackgroundBrush();

    _scene->addItem( new nodeWidget );
    _scene->addItem( new nodeWidget );
    _scene->addItem( new nodeWidget );
    _scene->addItem( new nodeWidget );
    }

void nodeExplorerWidget::resizeEvent( QResizeEvent * )
    {
    //refreshBackgroundBrush();
    }

void nodeExplorerWidget::refreshBackgroundBrush( )
    {
    /*QRadialGradient grad( viewportTransform().inverted().map( QPointF( width()/2, height()/2 ) ), 0.5 * sqrt( width()*width() + height()*height() ) );
    grad.setColorAt( 0, QApplication::palette().color( QPalette::Light ) );
    grad.setColorAt( 1, QApplication::palette().color( QPalette::Shadow ) );

    setBackgroundBrush( grad );*/
    }
