#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QFrame>
#include "XProperty"

class APlugin;

class timelineWidget : public QFrame
    {
    XROProperty( APlugin *, app )
    Q_OBJECT
public:
    timelineWidget( QWidget *parent=0, APlugin *app=0 );

    int currentFrame() const;
    int maximumFrame() const;
    int minimumFrame() const;
    double fps() const;

protected:
    virtual void keyPressEvent(QKeyEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent *);
    virtual void contextMenuEvent(QContextMenuEvent *);

public slots:
    void setCurrentFrame( int );
    void setMaximumFrame( int );
    void setMinimumFrame( int );
    void setFps( double );

signals:
    void currentTimeChanged( int );

private slots:
    void refreshDisplayFromArray();

private:
    int _currentFrame;
    int _startFrame;
    int _endFrame;
    float _frameSize;
    float _pos;
    float _fps;
    QPoint _oldMousePos;
    bool _dragging;
    bool _selecting;
    };

#endif // TIMELINEWIDGET_H
