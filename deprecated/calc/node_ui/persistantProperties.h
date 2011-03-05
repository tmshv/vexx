#ifndef PERSISTANTPROPERTIES_H
#define PERSISTANTPROPERTIES_H

#include "QGraphicsProxyWidget"
#include "XProperty"

class APlugin;
class QTextEdit;
class QStackedWidget;
class QToolButton;
class gfxview;
class QGraphicsSceneMouseEvent;
class persistantProperties;

class propertiesWidget : public QGraphicsObject
    {
    Q_OBJECT
    XROProperty( APlugin *, app )
public:
    enum { Type = UserType + 4 };
    propertiesWidget( APlugin *, gfxview * );

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
    virtual void keyPressEvent( QKeyEvent *event );

public slots:
    void toggleHidden();

private slots:
    void reformView();

private:
    virtual int type() const;
    persistantProperties *_prop;
    bool _dragging;
    bool _resizing;
    Qt::WindowFrameSection _section;
    };

class persistantProperties : public QGraphicsProxyWidget
    {
    Q_OBJECT
    XROProperty( APlugin *, app )
public:
    enum { Type = UserType + 5 };
    persistantProperties( APlugin *, QGraphicsItem * );

public slots:
    void triggerRebuild();

signals:
    void contentsChanged();

private slots:
    void add();
    void remove();
    void toggleHelp( bool );
    void refreshSelection();

private:
    virtual int type() const;
    QWidget *rebuild();
    QWidget *_properties;
    QTextEdit *_help;
    QStackedWidget *_stack;
    QToolButton *_helpToggle;
    QToolButton *_add;
    QToolButton *_remove;
    bool _dragging;
    };

#endif // PERSISTANTPROPERTIES_H
