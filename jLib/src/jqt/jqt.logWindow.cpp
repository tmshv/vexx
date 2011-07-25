#include "jqt/jqt.logWindow.h"
#include <QMessageBox>
#include <iostream>

namespace jLib
{
namespace jqt
{

logWindow::logWindow(QWidget *parent) : QDialog(parent), m_ui(new Ui::logWindow)
    {
    m_ui->setupUi(this);
    }

logWindow::~logWindow()
    {
    delete m_ui;
    }

int logWindow::handle( error::message in )
    {
    if( in.type >= 1000 )
        {
        std::ostringstream str;
        str<<"In file "<<in.file<<", in function "<<in.function<<", on line "<<in.line<<":"<<endl<<in.message<<endl;
        std::cerr<<str.str()<<std::endl;
        QMessageBox::critical( 0, "jLib Error", QString::fromStdString( str.str() ) );
        return error::ABORT;
        }
    else
        {
        if( !isVisible() )
            {
            show();
            }
        QString currentText = m_ui->logText->toPlainText();
        std::cout<<in.message<<std::endl;
        m_ui->logText->setText( currentText + QString::fromStdString( in.message ) + "\n" );
        return error::RETURN;
        }
    }

void logWindow::changeEvent(QEvent *e)
    {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

}
}
