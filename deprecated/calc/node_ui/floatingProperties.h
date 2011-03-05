#ifndef FLOATINGPROPERTIES_H
#define FLOATINGPROPERTIES_H

#include "QGraphicsObject"
#include "XProperty"

class nodeGraphicalDetails;
class nodeEntity;
class APropertyDevice;
class AProperty;
class QGraphicsProxyWidget;
class QTabWidget;
class QStackedWidget;
class QTextEdit;
class exportWidget;
class scriptEdit;

class floatingProperties : public QGraphicsObject
    {
    Q_OBJECT
public:
    enum { Type = UserType + 3 };
    enum displayMode { Properties, Developer, Help, Export };

    floatingProperties( nodeGraphicalDetails *parent );
    ~floatingProperties( );

    displayMode mode() const;
    void setMode( displayMode );

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    QRectF boundingRect() const;

    virtual int type() const;

protected:
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

private slots:
    void viewScaled();
    void reloadWidget();
    void sourceChanged();
    void editNode();
    void enterGroup();
    void editGroup();
    void entityChanged(APropertyDevice*,AProperty*);

private:
    void calculateTabs();
    struct tabStruct
        {
        tabStruct( QString t, QWidget *widg=0 ) : text( t ), widget( widg ) { }
        QString text;
        QString calculatedText;
        QRectF rect;
        QWidget *widget;
        };

    QList <tabStruct> _tabData;

    QColor textColour() const;
    void formatHeight();
    qreal _height;
    nodeGraphicalDetails *_parent;
    nodeEntity *_controller;
    QWidget *_widget;
    QGraphicsProxyWidget *_proxy;
    QStackedWidget *_stack;
    QWidget *_source;
    QTextEdit *_help;
    exportWidget *_renderExporter;
    scriptEdit *_script;
    float _oldHeight;
    QRectF _bRect;
    bool _settingSource;
    };

#endif // FLOATINGPROPERTIES_H
