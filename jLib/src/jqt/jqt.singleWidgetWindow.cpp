#include "jqt/jqt.singleWidgetWindow.h"

namespace jLib
{
namespace jqt
{

singleWidgetWindow::singleWidgetWindow( viewportWidgetBase *child, QWidget *parent ) : QMainWindow(parent), ui( new Ui::SingleWidgetWindow), logWin( 0 )
    {
    ui->setupUi(this);
    centralWidgetContents = child;
    centralWidgetContents->setParent( ui->centralWidget );
    ui->horizontalLayout->addWidget( centralWidgetContents, Qt::AlignCenter );
    //enableLogWindow();
    }

void singleWidgetWindow::enableLogWindow()
    {
    logWin = new logWindow( this );
    error::report::assignHandlerStatic( -1, logWin );
    }

singleWidgetWindow::~singleWidgetWindow()
    {
    if( logWin != 0 )
        {
        logWin->hide();
        delete logWin;
        }

    delete centralWidgetContents;
    delete ui;
    }

}
}
