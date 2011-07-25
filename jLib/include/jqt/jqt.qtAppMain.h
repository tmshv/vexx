#ifndef JLIB_JQT_QTAPPMAIN_H
#define JLIB_JQT_QTAPPMAIN_H

#include <QtGui/QApplication>

#define JLIB_JQT_BASEMAIN( X, Y ) int main( int argc, char *argv[] )    \
    {                                                                   \
    QApplication a(argc, argv);                                         \
    X w( Y );                                                           \
    w.show();                                                           \
    return a.exec();                                                    \
    }

#endif
