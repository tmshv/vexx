#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "jLib.h"

namespace Ui
{
    class MainWindow;
}

class propertyDevice
    {
public:
    enum typeEnum { String };
    struct propStruct { jLib::string name; typeEnum type; };

    void setProperty( jLib::string, jLib::string ) { jDebugNeat<<"Invalid Property Set Method Called"<<endl; }
    void addProperty( jLib::string n, typeEnum t )
        {
        propStruct p;
        p.name = n;
        p.type = t;
        _properties.push_back( p );
        }
    const jLib::vector <propStruct> properties() { return _properties; }

private:
    jLib::vector <propStruct> _properties;
    };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setProperties( propertyDevice & );
    void refreshProperties( );
    void clearProperties( );
    void addProperty( QString, QWidget * );

public slots:
    void propertyChanged();

private:
    propertyDevice someProp;
    propertyDevice *_device;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
