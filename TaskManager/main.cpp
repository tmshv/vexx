#include <QtGui/QApplication>
#include "mainwindow.h"
#include "sprocessmanager.h"

int main(int argc, char *argv[])
  {
  xInitiateMetaTypes();

  SProcessManager::initiate(1);

  QApplication a(argc, argv);

  // scope for main window
  int ret = 0;
    {
    MainWindow w;
    w.show();
    ret = a.exec();
    }

  SProcessManager::terminate();
  return ret;
  }
