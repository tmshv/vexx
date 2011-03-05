#ifndef NEWITEMDIALOG_H
#define NEWITEMDIALOG_H

#include <QDialog>

namespace Ui {
    class NewItemDialog;
}

class XEnvironment;

class NewItemDialog : public QDialog
  {
  Q_OBJECT

public:
  explicit NewItemDialog(XEnvironment *environment, QWidget *parent = 0);
  ~NewItemDialog();

private slots:
  void acceptNewItem();
  void chooseFilename();

private:
  Ui::NewItemDialog *ui;
  XEnvironment *_environment;
  };

#endif // NEWITEMDIALOG_H
