#include "UIDatabaseDebugSurface.h"
#include "QTreeView"
#include "QVBoxLayout"
#include "sdatabase.h"
#include "QHeaderView"
#include "QMenu"

UIDatabaseDebugSurface::UIDatabaseDebugSurface(SDatabase *db)
    : UISurface("Database Debug", new QWidget(), UISurface::Dock),
    _db(db), _model(_db, _db, SDatabaseModel::ShowValues)
  {
  QVBoxLayout *layout = new QVBoxLayout(widget());
  layout->setContentsMargins(2,2,2,2);

  _treeView = new QTreeView(widget());
  layout->addWidget(_treeView);

  _treeView->setModel(&_model);

  _treeView->setItemDelegate(&_delegate);
  _treeView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  _treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
  }

void UIDatabaseDebugSurface::setDatabase(SDatabase *db)
  {
  _db = db;
  _model.setDatabase(db, db);
  }

void UIDatabaseDebugSurface::contextMenu(QPoint point)
  {
  QModelIndex index = _treeView->indexAt(point);

  bool exec = false;
  QMenu menu;

  if(index.isValid())
    {
    _clickedItem = (SProperty *)index.internalPointer();

    if(_clickedItem->hasInput())
      {
      QAction *inputLabel = menu.addAction("Input");
      inputLabel->setEnabled(false);
      exec = true;

      menu.addAction(_clickedItem->input()->path(), this, SLOT(disconnectInput()));
      }

    if(_clickedItem->hasOutputs())
      {
      if(exec)
        {
        menu.addSeparator();
        }

      QAction *outputLabel = menu.addAction("Outputs");
      outputLabel->setEnabled(false);
      exec = true;

      SProperty *output = _clickedItem->output();
      while(output)
        {
        QAction *act = menu.addAction(output->path(), this, SLOT(disconnectOutput()));

        act->setData((qint64)output);

        output = output->nextOutput();
        }
      }

    SPropertyContainer *propContainer = _clickedItem->castTo<SPropertyContainer>();
    if(propContainer)
      {
      if(exec)
        {
        menu.addSeparator();
        }

      QMenu *createMenu = menu.addMenu("Create");
      }
    }

  menu.exec(_treeView->mapToGlobal(point));
  }

void UIDatabaseDebugSurface::disconnectInput()
  {
  _clickedItem->input()->disconnect(_clickedItem);
  }

void UIDatabaseDebugSurface::disconnectOutput()
  {
  if(sender())
    {
    QAction *act = qobject_cast<QAction*>(sender());

    if(act)
      {
      SProperty *prop = (SProperty*)act->data().value<qint64>();

      _clickedItem->disconnect(prop);
      }
    }
  }
