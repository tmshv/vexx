#include "QHBoxLayout"
#include "QFormLayout"
#include "QTreeView"
#include "QListWidget"
#include "QTextEdit"
#include "QDoubleSpinBox"
#include "QPushButton"
#include "QLineEdit"
#include "mainwindow.h"
#include "item.h"
#include "sdatabasemodel.h"
#include "QHeaderView"
#include "sproperty.h"
#include "QDir"
#include "QMenu"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    _dbModel(new SDatabaseModel(&_db, _db.rootItem(), SDatabaseModel::EntitiesOnly)), _item(0),
    _updateLinks(0), _nullifyItem(0)
  {
  _db.rootItem()->addTreeObserver(this);

  QWidget *taskWindow = new QWidget(this);
  this->setCentralWidget(taskWindow);

  QHBoxLayout *layout = new QHBoxLayout(taskWindow);
  layout->setContentsMargins(0,0,0,0);

  QVBoxLayout *leftLayout = new QVBoxLayout();
  layout->addLayout(leftLayout);

  // item tree
  _itemTree = new QTreeView(taskWindow);
  _itemTree->setHeaderHidden(true);
  _itemTree->setModel(_dbModel);
  connect(_itemTree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemChosen(QModelIndex)));
  _itemTree->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_itemTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
  leftLayout->addWidget(_itemTree);

  _title = new QLineEdit();
  _description = new QTextEdit();
  _priority = new QDoubleSpinBox();
  _severity = new QDoubleSpinBox();

  QPushButton *save = new QPushButton("&Save");
  connect(save, SIGNAL(clicked()), this, SLOT(save()));

  QFormLayout *selectedTask = new QFormLayout();
  layout->addLayout(selectedTask);
  selectedTask->addRow("Title", _title);
  selectedTask->addRow("Description", _description);
  selectedTask->addRow("Priority", _priority);
  selectedTask->addRow("Severity", _severity);

  // dependancy panel
  _links = new QListWidget(taskWindow);
  connect(_links, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemChosen(QModelIndex)));
  selectedTask->addRow(_links);

  selectedTask->addRow(save);

  _db.load();
  _dbModel->setRoot(_db.rootItem());
  }

MainWindow::~MainWindow()
  {
  _db.save();
  }

void MainWindow::itemChosen(QModelIndex index)
  {
  if(_item)
    {
    _item->removeConnectionObserver(this);
    }

  SEntity *ent(0);
  if(sender() == _itemTree)
    {
    ent = ((SProperty *)index.internalPointer())->castTo<SEntity>();
    }
  else
    {
    ent = reinterpret_cast<Item*>(_links->item(index.row())->data(50).toULongLong());
    }

  if(ent)
    {
    _title->setText(ent->name());

    _item = ent->castTo<Item>();
    if(_item)
      {
      _item->addConnectionObserver(this);
      }
    updateUIFromDB();
    }
  }

void MainWindow::updateUIFromDB()
  {
  if(_item)
    {
    updateLinks();
    _description->setText(_item->description());
    _priority->setValue(_item->priority());
    _severity->setValue(_item->severity());
    }
  else
    {
    _description->setText("");
    _priority->setValue(0.0);
    _severity->setValue(0.0);
    }
  }

void MainWindow::save()
  {
  if(_item)
    {
    _item->setName(_title->text());
    _item->description = _description->toPlainText();
    _item->severity = _severity->value();
    _item->priority = _priority->value();
    }

  updateUIFromDB();
  }

void MainWindow::contextMenu(QPoint pt)
  {
  _itemHit = _itemTree->indexAt(pt);

  QMenu menu;
  menu.addAction("Add Item", this, SLOT(addItem()));
  if(_itemHit.isValid())
    {
    Item *hitItem = ((SProperty*)_itemHit.internalPointer())->castTo<Item>();

    menu.addAction("Remove Item", this, SLOT(removeItem()));

    if(_item && hitItem != _item)
      {
      menu.addSeparator();

      if(!_item->links.hasPointer(hitItem))
        {
        menu.addAction("Add Dependancy", this, SLOT(addDependancy()));
        }
      else
        {
        menu.addAction("Remove Dependancy", this, SLOT(removeDependancy()));
        }
      }
    }
  menu.exec(_itemTree->mapToGlobal(pt));
  }

void MainWindow::addItem()
  {
  Item *p = 0;
  if(_itemHit.isValid())
    {
    p = ((SProperty*)_itemHit.internalPointer())->castTo<Item>();
    }
  Item *newItem = _db.addItem(p);

  newItem->priority.connect(&newItem->severity);
  }

void MainWindow::removeItem()
  {
  if(_itemHit.isValid())
    {
    Item *item = ((SProperty*)_itemHit.internalPointer())->castTo<Item>();
    _db.removeItem(item);
    }
  }

void MainWindow::addDependancy()
  {
  if(_itemHit.isValid() && _item)
    {
    Item *p = ((SProperty*)_itemHit.internalPointer())->castTo<Item>();
    if(p != _item)
      {
      _item->addDependancy(p);
      }
    }
  }

void MainWindow::removeDependancy()
  {
  if(_itemHit.isValid() && _item)
    {
    Item *p = ((SProperty*)_itemHit.internalPointer())->castTo<Item>();
    if(p != _item && _item->links.hasPointer(p))
      {
      _item->removeDependancy(p);
      }
    }
  }

void MainWindow::onTreeChange(int m, const SChange *ev)
  {
  const SPropertyArray::TreeChange *treeChange = ev->castTo<SPropertyArray::TreeChange>();
  if(treeChange)
    {
    if(_item == treeChange->before() || _item == treeChange->after())
      {
      _updateLinks = true;
      }

    if(_item == treeChange->property())
      {
      _nullifyItem = true;
      }
    }

  const SProperty::NameChange *nameChange = ev->castTo<SProperty::NameChange>();
  if(nameChange && nameChange->property() == _item)
    {
    _updateLinks = true;
    }
  }

void MainWindow::onConnectionChange(int m, const SChange *ev)
  {
  _updateLinks = true;
  }

void MainWindow::actOnChanges()
  {
  if(_updateLinks)
    {
    updateLinks();
    _updateLinks = false;
    }
  if(_nullifyItem)
    {
    _item->removeConnectionObserver(this);
    _item = 0;
    updateUIFromDB();
    _nullifyItem = false;
    }
  }

void MainWindow::updateLinks()
  {
  _links->clear();
  if(_item)
    {
    SProperty *item = _item->links.firstChild();
    while(item)
      {
      SProperty *input = item->input();
      if(input)
        {
        Item *inputItem = input->castTo<Item>();
        if(inputItem)
          {
          QListWidgetItem *tItem = new QListWidgetItem();
          tItem->setData(50,reinterpret_cast<xuint64>(inputItem));
          tItem->setText(inputItem->name());
          _links->addItem(tItem);
          }
        }
      item = item->nextSibling();
      }
    }
  }
