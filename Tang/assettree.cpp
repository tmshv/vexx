#include "assettree.h"
#include "QWidget"
#include "QTreeView"
#include "QVBoxLayout"
#include "QHeaderView"
#include "XEnvironment.h"
#include "application.h"
#include "QMenu"
#include "QAction"

AssetTree::AssetTree(Application *app) : UISurface("Assets", new QWidget(0), UISurface::Dock), _app(app)
  {
  QVBoxLayout *layout = new QVBoxLayout(widget());
  layout->setContentsMargins(2,2,2,2);

  _treeView = new QTreeView(widget());
  layout->addWidget(_treeView);

  _treeView->setModel(this);

  _treeView->header()->hide();
  //_treeView->setItemDelegate(&_delegate);
  //_treeView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  _treeView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));

  connect(_app, SIGNAL(requestComplete(XEnvironment::Request)), this, SLOT(updateFromRequest(XEnvironment::Request)));
  connect(_app, SIGNAL(dataChanged(XEnvironment::Request)), this, SLOT(updateFromRequest(XEnvironment::Request)));
  }

void AssetTree::updateFromRequest(const XEnvironment::Request &r)
  {
  if(r.type() == XEnvironment::ContainerType)
    {
    const XEnvironment::Container *cont = _app->environment().container(r.ID());
    if(cont)
      {
      const XEnvironment::Container *parent = 0;
      xuint32 index = 0;
      if(r.ID() != 0)
        {
        parent = _app->environment().container(cont->parentID());
        xAssert(parent);
        if(parent)
          {
          index = parent->indexOf(cont);
          }
        }

      if(index == X_SIZE_SENTINEL)
        {
        return;
        }

      QModelIndex changedIndex = createIndex(index, 0, (void *)r.ID());

      emit layoutAboutToBeChanged();
      emit layoutChanged();
      emit dataChanged(changedIndex, changedIndex);
      }

    }
  }

void AssetTree::contextMenu(QPoint p)
  {
  XEnvironment::ItemID parent = 0;
  QModelIndex index = _treeView->indexAt(p);
  if(index.isValid())
    {
    parent = index.internalId();
    }

  QMenu menu;
  QAction *act = menu.addAction(QIcon(":/tang/resources/Folder.svg"), "New Folder");
  act->setData(parent);

  connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(contextMentTriggered(QAction*)));
  menu.exec(_treeView->mapToGlobal(p));
  }

void AssetTree::contextMentTriggered(QAction *act)
  {
  if(act->text() == "New Folder")
    {
    _app->createContainer(act->data().toULongLong());
    }
  }

void AssetTree::requestContainer(XEnvironment::ItemID id) const
  {
  XEnvironment::Request r(XEnvironment::ContainerType, id);
  _app->environment().requestItem(r);
  }

int AssetTree::rowCount(const QModelIndex &parent) const
  {
  bool request = false;
  if(!parent.isValid())
    {
    const XEnvironment::Container *cont = _app->environment().container(0);
    if(cont)
      {
      return 1;
      }
    else
      {
      request = true;
      }
    }

  if(!request)
    {
    const XEnvironment::Container *cont = _app->environment().container(parent.internalId());
    if(cont)
      {
      bool allLoaded = true;
      foreach(XEnvironment::ItemID id, cont->items())
        {
        const XEnvironment::Container *child = _app->environment().container(id);
        if(!child)
          {
          requestContainer(id);
          allLoaded = false;
          }
        }

      if(!allLoaded)
        {
        return 0;
        }
      else
        {
        return cont->items().size();
        }
      }
    }

  if(request)
    {
    requestContainer(parent.internalId());
    }
  return 0;
  }

QModelIndex AssetTree::index(int row, int column, const QModelIndex &parent) const
  {
  bool request = false;

  if(parent.isValid())
    {
    const XEnvironment::Container *cont = _app->environment().container(parent.internalId());
    xAssert(cont);
    if(cont)
      {
      xAssert(row < cont->items().size());
      if(row < cont->items().size())
        {
        const XEnvironment::Container *child = _app->environment().container(cont->items()[row]);
        xAssert(child);

        return createIndex(row, column, (void*)cont->items()[row]);
        }
      }
    else
      {
      request = true;
      }
    }
  else
    {
    const XEnvironment::Container *cont = _app->environment().container(0);
    if(cont)
      {
      return createIndex(row, column, (void*)0);
      }
    else
      {
      request = true;
      }
    }

  if(request)
    {
    requestContainer(parent.isValid() ? parent.internalId() : 0);
    }

  return QModelIndex();
  }

QModelIndex AssetTree::parent(const QModelIndex &child) const
  {
  xAssert(child.isValid());
  if(!child.isValid() || child.internalId() == 0)
    {
    return QModelIndex();
    }

  const XEnvironment::Container *cont = _app->environment().container(child.internalId());
  if(cont)
    {
    const XEnvironment::Container *parent = _app->environment().container(cont->parentID());
    xAssert(parent);

    xuint32 row = 0;
    if(parent && parent->ID() != 0)
      {
      const XEnvironment::Container *grandParent = _app->environment().container(parent->parentID());

      if(grandParent)
        {
        row = grandParent->indexOf(parent);

        xAssert(row != X_SIZE_SENTINEL);
        if(row == X_SIZE_SENTINEL)
          {
          row = 0;
          }
        }
      }

    return createIndex(row, child.column(), (void *)parent->ID());
    }

  return QModelIndex();
  }

int AssetTree::columnCount(const QModelIndex &parent) const
  {
  bool request = false;
  if(!parent.isValid())
    {
    const XEnvironment::Container *cont = _app->environment().container(0);
    if(cont)
      {
      return 1;
      }
    else
      {
      request = true;
      }
    }

  if(!request)
    {
    const XEnvironment::Container *cont = _app->environment().container(parent.internalId());
    if(cont)
      {
      return 1;
      }
    else
      {
      request = true;
      }
    }

  if(request)
    {
    requestContainer(parent.internalId());
    }
  return 0;
  }

QVariant AssetTree::data(const QModelIndex &index, int role) const
  {
  if(role == Qt::DisplayRole && index.column() == 0)
    {
    if(!index.isValid())
        {
        return "";
        }

      const XEnvironment::Container *cont = _app->environment().container(index.internalId());
      if(cont)
        {
        return cont->name();
        }
    }

  return QVariant();
  }
