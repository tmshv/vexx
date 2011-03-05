#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
    {
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    float h;
    };

#endif // WIDGET_H
