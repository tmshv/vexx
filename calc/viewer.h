#ifndef APPVIEWPORT_H
#define APPVIEWPORT_H

#define find3DEntities findHasProperty( "viewport/object" )

#include "AGlobal"
#include "ALog"
#include "AEntityArray"
#include "AAbstractDock"
#include "nodeRenderer.h"
#include "jLib.h"

class nodeEntity;

class viewer : public QGLWidget
    {
    Q_OBJECT
    jROProperty( APlugin *, app );
    jROProperty( bool, isHorizontallySplit );
    jROProperty( bool, isSplitEnabled );
public:
    enum ColourPickerMode { None, Picking, Moving, Deleting };
    viewer( QWidget *, APlugin * );
    ~viewer();

    void translateView( QPointF transVector );
    void zoomView( float zF, QPointF point );

    void setDisplayMode( nodeRenderer::DisplayModeEnum );

    bool isColourSampling() const;
    bool shouldPreMultiply() const;

    void clearCache();
    void setCache( int, QImage );

public slots:
    void newColourPicker();
    void setGlobalMultiplier( double );
    void setPreMultiply( bool );
    void setSplitEnabled( bool );
    void setHorizontallySplit( bool );
    void setDisplayTime( int );
    void resetView();
    void oneToOne();
    void zoomToFit();

signals:
    void colourSampleChanged( XColour );

protected:
    void resizeEvent( QResizeEvent * );
    void mousePressEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void wheelEvent( QWheelEvent * );
    void contextMenuEvent( QContextMenuEvent * );
    void focusOutEvent( QFocusEvent * );

private slots:
    void refreshViewNodeFromArray();
    void checkNodes();
    void setViewNode( nodeEntity *, int );
    void entityAdded( AEntity * );
    void entityRemoved( AEntity * );
    void updateViewTransform();
    void initiateManipulators();
    void clearViewer();

private:
    void resizeView( float x, float y );
    QTransform fullTransform();
    QTransform aspectTransform();
    void setViewTransform();
    void paintEvent(QPaintEvent *event);
    QTransform _painterTransform;
    QTransform _mouseTransform;
    QPointF _previousCoord;
    QPointF _zoomTarget;
    QPoint _oldPos;
    bool _transformNeedsUpdate;
    bool _isSampling;

    bool _rebuild;
    nodeRenderer _renderer;
    nodeRenderer _rendererSecondary;
    QTransform _globalAdjustment;
    QTransform _persp;
    QTransform _view;

    nodeEntity *_activeNode;
    int _activeManipulator;
    bool _transforming;
    QPointF _splitPos;
    bool _splitPosChanging;

    QList <QPoint> _colourSamplers;
    QList <QPoint> _colourSamplerPositions;
    ColourPickerMode _activePickerMode;
    int _activePicker;

    int _frame;
    QMap <int, QImage> _cache;
    };

#endif
