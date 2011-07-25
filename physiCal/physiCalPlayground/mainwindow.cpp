#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLineEdit"
#include "QLabel"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
    {
    ui->setupUi(this);

    someProp.addProperty( "hello", propertyDevice::String );
    setProperties( someProp );
    }

void MainWindow::setProperties( propertyDevice &d )
    {
    _device = &d;
    refreshProperties();
    }

void MainWindow::refreshProperties( )
    {
    clearProperties();
    propertyDevice::propStruct prop;
    foreach( prop, _device->properties() )
        {
        QWidget *widget;
        if( prop.type == propertyDevice::String )
            {
            widget = new QLineEdit( );
            connect( widget, SIGNAL( editingFinished() ), this, SLOT( propertyChanged() ) );
            }
        else
            {
            widget = new QLabel( "Invalid property type" );
            }
        addProperty( QString::fromStdString( prop.name ), widget );
        }
    }

void MainWindow::clearProperties( )
    {
    for( int x=0; x<ui->formLayout->rowCount(); x++ )
        {
        QLayoutItem *label = ui->formLayout->itemAt( x, QFormLayout::LabelRole );
        QLayoutItem *field = ui->formLayout->itemAt( x, QFormLayout::FieldRole );

        ui->formLayout->removeItem( label );
        ui->formLayout->removeItem( field );
        label->widget()->deleteLater();
        field->widget()->deleteLater();
        }
    }

void MainWindow::addProperty( QString name, QWidget *widget )
    {
    widget->setParent( ui->propertiesLayout );
    ui->formLayout->addRow( name, widget );
    }

void MainWindow::propertyChanged()
    {
    for( int x=0; x<ui->formLayout->rowCount(); x++ )
        {
        if( _device->properties()[x].type == propertyDevice::String )
            {
            _device->setProperty( static_cast <QLabel> (ui->formLayout->itemAt( x, QFormLayout::LabelRole )->widget()).text().toStdString(),
                                  static_cast <QLineEdit> (ui->formLayout->itemAt( x, QFormLayout::FieldRole )->widget()).text().toStdString() );
            }
        }
    }

MainWindow::~MainWindow()
    {
    delete ui;
    }
