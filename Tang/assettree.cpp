#include "assettree.h"
#include "QWidget"
#include "QTreeView"
#include "QVBoxLayout"
#include "QHeaderView"
#include "XEnvironment.h"
#include "application.h"
#include "QMenu"
#include "QTimerEvent"
#include "QAction"

AssetTree::AssetTree(Application *app) : UISurface("Assets", new QWidget(0), UISurface::Dock), _app(app),
    _containerIcon(":/tang/resources/Folder.svg"), _textureIcon(":/tang/resources/Texture.svg"),
    _cleanupTimerID(-1)
  {
  QVBoxLayout *layout = new QVBoxLayout(widget());
  layout->setContentsMargins(2,2,2,2);

  _treeView = new QTreeView(widget());
  layout->addWidget(_treeView);

  _treeView->setModel(this);

  _treeView->header()->hide();
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
      xuint32 idx = 0;
      if(r.ID() != 0)
        {
        parent = _app->environment().container(cont->parentID());
        xAssert(parent);
        if(parent)
          {
          idx = parent->indexOf(cont);
          }
        }

      if(idx == X_SIZE_SENTINEL)
        {
        return;
        }

      QModelIndex changedIndex = createIndex(idx, 0, indexForID(r.type(), r.ID()));

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
    parent = containerID(index);
    }

  QMenu menu;

  QAction *folder = menu.addAction(_containerIcon, "New Folder");
  folder->setData(parent);

  QAction *texture = menu.addAction(_textureIcon, "New Texture");
  texture->setData(parent);

  connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(contextMentTriggered(QAction*)));
  menu.exec(_treeView->mapToGlobal(p));
  }

void AssetTree::contextMentTriggered(QAction *act)
  {
  if(act->text() == "New Folder")
    {
    _app->createContainer(act->data().toULongLong());
    }
  else if(act->text() == "New Texture")
    {
    _app->createTexture(act->data().toULongLong());
    }
  }

void AssetTree::requestContainer(XEnvironment::ItemID id) const
  {
  XEnvironment::Request r(XEnvironment::ContainerType, id);
  _app->environment().requestItem(r);
  }

int AssetTree::rowCount(const QModelIndex &parent) const
  {
  if(!parent.isValid())
    {
    const XEnvironment::Container *cont = _app->environment().container(0);
    if(cont)
      {
      return 1;
      }
    else
      {
      requestContainer(0);
      return 0;
      }
    }

  if(!isContainer(parent))
    {
    return 0;
    }

  const XEnvironment::Container *cont = _app->environment().container(containerID(parent));
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

  requestContainer(containerID(parent));
  return 0;
  }

XEnvironment::ItemID AssetTree::containerID(const QModelIndex &index) const
  {
  xAssert(index.internalId() < _indexMap.size());
  if(index.internalId() < _indexMap.size())
    {
    return _indexMap[index.internalId()].id;
    }
  return 0;
  }

bool AssetTree::isContainer(const QModelIndex &index) const
  {
  xAssert(index.internalId() < _indexMap.size());
  if(index.internalId() < _indexMap.size())
    {
    return _indexMap[index.internalId()].type == XEnvironment::ContainerType;
    }
  return false;
  }

bool AssetTree::isTexture(const QModelIndex &index) const
  {
  xAssert(index.internalId() < _indexMap.size());
  if(index.internalId() < _indexMap.size())
    {
    return _indexMap[index.internalId()].type == XEnvironment::TextureType;
    }
  return false;
  }

quint32 AssetTree::indexForID(xuint16 type, XEnvironment::ItemID id) const
  {
  Index index;
  index.id = id;
  index.type = type;

  int i = _indexMap.indexOf(index);
  if(i == -1)
    {
    i = _indexMap.size();
    _indexMap << index;

    if(_cleanupTimerID != -1)
      {
      ((AssetTree*)this)->killTimer(_cleanupTimerID);
      }
    _cleanupTimerID = ((AssetTree*)this)->startTimer(60000);
    }
  return i;
  }

void AssetTree::timerEvent(QTimerEvent *event)
  {
  if(event->timerId() == _cleanupTimerID)
    {
    killTimer(_cleanupTimerID);
    _cleanupTimerID = -1;

    emit layoutAboutToBeChanged();

    QModelIndexList	pers = persistentIndexList();
    for(int i=0, s=_indexMap.size(); i<s; ++i)
      {
      bool found = false;

      foreach(const QModelIndex &idx, pers)
        {
        if(idx.internalId() == i)
          {
          found = true;
          }
        }

      if(!found)
        {
        _indexMap.erase(_indexMap.begin()+i);
        --i;
        --s;
        }
      }

    emit layoutChanged();
    }
  }

QModelIndex AssetTree::index(int row, int column, const QModelIndex &parent) const
  {
  if(parent.isValid())
    {
    xAssert(isContainer(parent));
    const XEnvironment::Container *cont = _app->environment().container(containerID(parent));
    xAssert(cont);
    if(cont)
      {
      xAssert(row < cont->items().size());
      if(row < cont->items().size())
        {
        const XEnvironment::Container *child = _app->environment().container(cont->items()[row]);
        xAssert(child);

        return createIndex(row, column, indexForID(XEnvironment::ContainerType, cont->items()[row]));
        }
      }
    else
      {
      requestContainer(containerID(parent));
      }
    }
  else
    {
    const XEnvironment::Container *cont = _app->environment().container(0);
    if(cont)
      {
      return createIndex(row, column, indexForID(XEnvironment::ContainerType, 0));
      }
    else
      {
      requestContainer(0);
      }
    }

  return QModelIndex();
  }

QModelIndex AssetTree::parent(const QModelIndex &child) const
  {
  xAssert(child.isValid());
  if(!child.isValid() || (isContainer(child) && containerID(child) == 0))
    {
    return QModelIndex();
    }

  xAssert(isContainer(child));
  const XEnvironment::Container *cont = _app->environment().container(containerID(child));
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

    return createIndex(row, child.column(), indexForID(XEnvironment::ContainerType, parent->ID()));
    }

  return QModelIndex();
  }

int AssetTree::columnCount(const QModelIndex &parent) const
  {
  if(!parent.isValid())
    {
    const XEnvironment::Container *cont = _app->environment().container(0);
    if(cont)
      {
      return 1;
      }
    else
      {
      requestContainer(0);
      return 0;
      }
    }

  if(!isContainer(parent))
    {
    return 1;
    }

  const XEnvironment::Container *cont = _app->environment().container(containerID(parent));
  if(cont)
    {
    return 1;
    }

  requestContainer(containerID(parent));
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

      if(isContainer(index))
        {
        const XEnvironment::Container *cont = _app->environment().container(containerID(index));
        if(cont)
          {
          return cont->name();
          }
        }
      else
        {
        return "NUTS";
        }
    }
  else if(role == Qt::DecorationRole && index.column() == 0)
    {
    if(!index.isValid())
        {
        return QIcon();
        }

      if(isContainer(index))
        {
        return _containerIcon;
        }
      else if(isTexture(index))
        {
        return _textureIcon;
        }
    }

  return QVariant();
  }
