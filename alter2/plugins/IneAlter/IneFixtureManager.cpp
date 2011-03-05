#include "IneFixtureManager.h"
#include "QWidget"
#include "QVBoxLayout"
#include "QListWidget"
#include "QMenu"
#include "sentity.h"
#include "IneFixture.h"
#include "IneFixtureEditDialog.h"
#include "sdatabase.h"

IneFixtureManager::IneFixtureManager(SEntity *fixtureRoot) : UISurface("Fixture Manager", new QWidget, Dock), _root(0)
  {
  QVBoxLayout *layout = new QVBoxLayout(widget());
  layout->setContentsMargins(2,2,2,2);

  _list = new QListWidget(widget());
  _list->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
  layout->addWidget(_list);

  setRoot(fixtureRoot);

  _editFixtureDialog = new IneFixtureEditDialog(widget());
  }

void IneFixtureManager::setRoot(SEntity *root)
  {
  _root = root;
  _root->addTreeObserver(this);
  refreshUI();
  }

void IneFixtureManager::contextMenu(QPoint pt)
  {
  QMenu menu;
  menu.addAction("New Fixture", this, SLOT(newFixture()));

  if(_list->selectedItems().size() == 1)
    {
    menu.addAction("Edit Fixture", this, SLOT(editFixture()));
    }

  if(_list->selectedItems().size() > 0)
    {
    menu.addAction("Delete Fixture", this, SLOT(deleteFixture()));
    }
  menu.exec(_list->mapToGlobal(pt));
  }

void IneFixtureManager::newFixture()
  {
  IneFixture *newFix = _root->database()->createEntity<IneFixture>("New Fixture", _root);
  _editFixtureDialog->setFixture(newFix);
  _editFixtureDialog->show();
  refreshUI();
  connect(_editFixtureDialog, SIGNAL(finished(int)), this, SLOT(refreshUI()));
  }

void IneFixtureManager::editFixture()
  {
  QList<QListWidgetItem*> items = _list->selectedItems();
  if(items.size() > 0)
    {
    IneFixture *child = (IneFixture*)items[0]->data(100).toULongLong();
    if(child)
      {
      _editFixtureDialog->setFixture(child);
      _editFixtureDialog->show();
      refreshUI();
      }
    }
  }

void IneFixtureManager::deleteFixture()
  {
  foreach(QListWidgetItem *item, _list->selectedItems())
    {
    IneFixture *child = (IneFixture*)item->data(100).toULongLong();
    if(child)
      {
      _root->deleteChild(child);
      }
    }
  }

void IneFixtureManager::onTreeChange(const STreeEvent &)
  {
  refreshUI();
  }

void IneFixtureManager::refreshUI()
  {
  _list->clear();
  foreach(SEntity *child, _root->children())
    {
    IneFixture *fix = child->castTo<IneFixture>();
    if(fix)
      {
      QListWidgetItem *item = new QListWidgetItem(QString("[%1] %2").arg(fix->manufacturer()).arg(fix->name()));
      item->setData(100, (xuint64)fix);
      _list->addItem(item);
      }
    }
  }
