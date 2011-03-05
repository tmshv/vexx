#ifndef CUSTOMDOCK_H
#define CUSTOMDOCK_H

#include "QDockWidget"

/** \cond INTERNAL
  */

class QBoxLayout;
class AAbstractDock;
class QToolButton;

class APrivateCustomTitleBar : public QWidget
    {
    Q_OBJECT
public:
    APrivateCustomTitleBar( QWidget *parent = 0 );
    ~APrivateCustomTitleBar();
    void paintEvent( QPaintEvent * );
    void resizeEvent( QResizeEvent * );
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
private slots:
    void floatClick();

private:
    bool vertical;
    QBoxLayout *_layout;
    QToolButton *_popOut;
    QPixmap *_bg;
    };

class APrivateCustomDockWidget : public QDockWidget
    {
public:
    APrivateCustomDockWidget( AAbstractDock *in, QWidget *parent );
    void contextMenuEvent( QContextMenuEvent *event );
private:
    AAbstractDock *_dock;
    };

/** \endcond
  */

#endif // CUSTOMDOCK_H
