#include <QtGui/QApplication>
#include "surface.h"

int main(int argc, char *argv[])
  {
  QApplication a(argc, argv);
  Surface w, v;
  w.show();
  v.show();

  return a.exec();
  }
