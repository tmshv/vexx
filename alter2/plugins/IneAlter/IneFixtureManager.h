#ifndef INEFIXTUREMANAGER_H
#define INEFIXTUREMANAGER_H

#include "UISurface.h"
#include "sevent.h"

class QListWidget;
class IneFixtureEditDialog;

class IneFixtureManager : public QObject, public UISurface, private STreeObserver
  {
  Q_OBJECT
public:
  IneFixtureManager(SEntity *root);

  void setRoot(SEntity *root);

private slots:
  void contextMenu(QPoint);
  void newFixture();
  void editFixture();
  void deleteFixture();
  void refreshUI();

private:
  void onTreeChange(const STreeEvent &);

  SEntity *_root;
  QListWidget *_list;
  IneFixtureEditDialog *_editFixtureDialog;
  };

#endif // INEFIXTUREMANAGER_H
