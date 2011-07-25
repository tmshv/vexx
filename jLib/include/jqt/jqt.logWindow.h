#ifndef JLIB_JQT_LOGWINDOW_H
#define JLIB_JQT_LOGWINDOW_H

#include <QtGui/QDialog>
#include "error/error.report.h"

// Start UI File

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_logWindow
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *logText;

    void setupUi(QDialog *logWindow)
    {
        if (logWindow->objectName().isEmpty())
            logWindow->setObjectName(QString::fromUtf8("logWindow"));
        logWindow->resize(400, 300);
        verticalLayout = new QVBoxLayout(logWindow);
        verticalLayout->setMargin(2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        logText = new QTextEdit(logWindow);
        logText->setObjectName(QString::fromUtf8("logText"));
        logText->setEnabled(true);
        logText->setReadOnly(true);

        verticalLayout->addWidget(logText);


        retranslateUi(logWindow);

        QMetaObject::connectSlotsByName(logWindow);
    } // setupUi

    void retranslateUi(QDialog *logWindow)
    {
        logWindow->setWindowTitle(QApplication::translate("logWindow", "Log", 0, QApplication::UnicodeUTF8));
        logText->setDocumentTitle(QString());
        Q_UNUSED(logWindow);
    } // retranslateUi

};

namespace Ui {
    class logWindow: public Ui_logWindow {};
} // namespace Ui

QT_END_NAMESPACE

// End UI File
namespace jLib
{
namespace jqt
{

class logWindow : public QDialog, public error::reportHandler {
    Q_OBJECT
public:
    logWindow(QWidget *parent = 0);
    virtual ~logWindow();

    virtual int handle( error::message );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::logWindow *m_ui;
};

}
}

#endif
