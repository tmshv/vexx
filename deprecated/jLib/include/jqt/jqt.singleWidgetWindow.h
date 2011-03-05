#ifndef JLIB_JQT_SINGLEWIDGETWINDOW_H
#define JLIB_JQT_SINGLEWIDGETWINDOW_H

#include "jqt/jqt.viewportWidgetBase.h"
#include "jqt/jqt.logWindow.h"
#include <QtGui/QMainWindow>

// Begin UI File, copied to make externally including things easier

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SingleWidgetWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;

    void setupUi(QMainWindow *SingleWidgetWindow)
    {
        if (SingleWidgetWindow->objectName().isEmpty())
            SingleWidgetWindow->setObjectName(QString::fromUtf8("SingleWidgetWindow"));
        SingleWidgetWindow->resize(600, 400);
        centralWidget = new QWidget(SingleWidgetWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setMargin(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        SingleWidgetWindow->setCentralWidget(centralWidget);

        retranslateUi(SingleWidgetWindow);

        QMetaObject::connectSlotsByName(SingleWidgetWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SingleWidgetWindow)
    {
        SingleWidgetWindow->setWindowTitle(QApplication::translate("SingleWidgetWindow", "", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SingleWidgetWindow);
    } // retranslateUi

};

namespace Ui {
    class SingleWidgetWindow: public Ui_SingleWidgetWindow {};
} // namespace Ui

QT_END_NAMESPACE

// End UI File

namespace jLib
{
namespace jqt
{

class JLIB_EXPORT singleWidgetWindow : public QMainWindow
    {
    Q_OBJECT

public:
    singleWidgetWindow( viewportWidgetBase *child, QWidget *parent=0 );
    ~singleWidgetWindow();
    void enableLogWindow();

private:
    viewportWidgetBase *centralWidgetContents;
    Ui::SingleWidgetWindow *ui;
    logWindow *logWin;
    };

}
}


#define SINGLEWIDGETAPP( X ) JLIB_JQT_BASEMAIN( jLib::jqt::singleWidgetWindow, new X() )

#endif
