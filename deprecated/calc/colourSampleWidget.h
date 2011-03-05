#ifndef COLOURSAMPLEWIDGET_H
#define COLOURSAMPLEWIDGET_H

#include <QWidget>
#include "XColour"

class colourSampleWidget : public QWidget
    {
    Q_OBJECT
public:
    colourSampleWidget( QWidget *parent=0 );

public slots:
    void setColour(XColour);

protected:
    void paintEvent(QPaintEvent *);

private:
    XColour _col;
    };

#endif // COLOURSAMPLEWIDGET_H
