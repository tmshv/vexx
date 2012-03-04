#include "UIDatabaseDebugSurface.h"
#include "QTreeView"
#include "QVBoxLayout"
#include "sdatabase.h"
#include "QHeaderView"
#include "QMenu"
#include "QClipboard"
#include "QApplication"
#include "QFileDialog"
#include "Serialisation/sjsonio.h"

UIDatabaseDebugSurface::UIDatabaseDebugSurface(SDatabase *db)
    : UISurface("Database Debug", new QWidget(), UISurface::Dock),
    _db(db), _model(_db, _db, SDatabaseModel::ShowValues)
  {
  QVBoxLayout *layout = new QVBoxLayout(widget());
  layout->setContentsMargins(2,2,2,2);

  _treeView = new QTreeView(widget());
  layout->addWidget(_treeView);

  _treeView->setModel(&_model);

  _delegate = new SDatabaseDelegate(this);
  _treeView->setItemDelegate(_delegate);
  _treeView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  _treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
  }

UIDatabaseDebugSurface::~UIDatabaseDebugSurface()
  {
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

    menu.addAction(_clickedItem->isDirty() ? "Dirty" : "Not Dirty", this, SLOT(swapDirty()));

    QString path = _clickedItem->path();
    menu.addAction("Copy Path \"" + path + "\"", this, SLOT(copyPath()));
    menu.addAction("Save As JSON", this, SLOT(saveProperty()));

    if(_clickedItem->castTo<SPropertyContainer>())
      {
      menu.addAction("Load JSON Under This", this, SLOT(loadProperty()));
      }

    menu.addSeparator();
    exec = true;

    if(_clickedItem->hasInput())
      {
      QAction *inputLabel = menu.addAction("Input");
      inputLabel->setEnabled(false);

      menu.addAction(_clickedItem->input()->path(), this, SLOT(disconnectInput()));
      }

    if(_clickedItem->hasOutputs())
      {
      if(_clickedItem->hasInput())
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

      //QMenu *createMenu = menu.addMenu("Create");
      }
    }

  menu.exec(QCursor::pos());
  }

void UIDatabaseDebugSurface::saveProperty()
  {
  QString fname = QFileDialog::getSaveFileName(widget(), "Save Property as JSON");
  QFile f(fname);
  if(f.open(QIODevice::WriteOnly))
    {
    SJSONSaver j;
    j.setAutoWhitespace(true);
    j.writeToDevice(&f, _clickedItem->entity(), true);
    }
  }

void UIDatabaseDebugSurface::loadProperty()
  {
  QString fname = QFileDialog::getOpenFileName(widget(), "Open JSON Under Property");
  QFile f(fname);
  if(f.open(QIODevice::ReadOnly))
    {
    SJSONLoader j;
    j.readFromDevice(&f, _clickedItem->castTo<SPropertyContainer>());
    }
  }

void UIDatabaseDebugSurface::copyPath()
  {
  QString path = _clickedItem->path();

  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(path);
  }

void UIDatabaseDebugSurface::swapDirty()
  {
  if(_clickedItem)
    {
    _clickedItem->postSet();
    }
  else
    {
    _clickedItem->update();
    }
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
