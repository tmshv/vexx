#ifndef APRIVATESPLASHSCEEN_H
#define APRIVATESPLASHSCEEN_H

#include "QWidget"

class APrivateSplashScreen : public QWidget
    {
    Q_OBJECT
public:
    APrivateSplashScreen( QIcon icon, QImage splash );
    void paintEvent( QPaintEvent * );
    void finish( QWidget * );

signals:
    void finished();

protected:
    void showEvent( QShowEvent * );

private slots:
    void fadeTo( int );
    void fadeOut();
    void out();
    void in();

private:
    enum fadeDirection { In, Out };
    fadeDirection direction;
    void fadeIn();
    QPixmap map;
    bool fading;
    bool instant;
    };

#endif // APRIVATESPLASHSCEEN_H
