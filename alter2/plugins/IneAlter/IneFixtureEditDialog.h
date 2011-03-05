#ifndef INEFIXTUREEDITDIALOG_H
#define INEFIXTUREEDITDIALOG_H

#include "QDialog"
#include "sevent.h"

class IneFixture;
class QTabWidget;
class SEntityUI;

class IneFixtureEditDialog : public QDialog, public SDataObserver
  {
  Q_OBJECT
public:
  IneFixtureEditDialog(QWidget *parent=0);
  ~IneFixtureEditDialog();

  void setFixture(IneFixture *);

private slots:
  void add();
  void remove();
  void setType( int);

private:
  void refreshUI();
  void onDataChange( const SDataEvent & );

  IneFixture *_fixture;
  QTabWidget *_tabs;
  QWidget *_fixtureValueHolder;
  SEntityUI *_ui;
  };

#endif // INEFIXTUREEDITDIALOG_H
