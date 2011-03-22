#include "sdatabasemodel.h"
#include "sentity.h"
#include "sdatabase.h"
#include "spropertydata.h"
#include "QPushButton"
#include "QStyleOptionViewItem"

SDatabaseDelegate::SDatabaseDelegate() : _currentWidget(0)
  {
  }

QWidget *SDatabaseDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
  {
  _currentIndex = index;
  if(index.isValid())
    {
    SProperty *prop = (SProperty *)index.internalPointer();
    _currentWidget = _ui.createControlWidget(prop, parent);
    connect(_currentWidget, SIGNAL(destroyed(QObject *)), this, SLOT(currentItemDestroyed()));
    emit ((SDatabaseDelegate*)this)->sizeHintChanged(_currentIndex);
    return _currentWidget;
    }
  return 0;
  }

void SDatabaseDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
  {
  }

void SDatabaseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
  {
  }

QSize SDatabaseDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
  {
  if(index == _currentIndex)
    {
      xAssert(_currentWidget);
      if(_currentWidget)
      {
        return _currentWidget->sizeHint();
      }
    }
  return QItemDelegate::sizeHint(option, index);
  }

void SDatabaseDelegate::currentItemDestroyed()
  {
  emit ((SDatabaseDelegate*)this)->sizeHintChanged(_currentIndex);
  _currentIndex = QModelIndex();
  _currentWidget = 0;
  }

SDatabaseModel::SDatabaseModel(SDatabase *db, SEntity *ent, Options options) : _db(db), _root(ent),
    _options(options)
  {
  if(_root == 0)
    {
    _root = db;
    }

  if(_root)
    {
    _root->addTreeObserver(this);
    }
  }

int SDatabaseModel::rowCount( const QModelIndex &parent ) const
  {
  const SProperty *prop = _root;
  int size = 0;
  if(parent.isValid())
    {
    prop = (SProperty *)parent.internalPointer();
    }

  if(!prop)
    {
    return 0;
    }

  if(_options.hasFlag(EntitiesOnly))
    {
    const SEntity *ent = prop->castTo<SEntity>();
    xAssert(ent);

    prop = &ent->children;
    }

  const SPropertyContainer *container = prop->castTo<SPropertyContainer>();
  if(container)
    {
    SProperty *child = container->firstChild();
    while(child)
      {
      size++;
      child = child->nextSibling();
      }
    }

  return size;
  }

QModelIndex SDatabaseModel::index( int row, int column, const QModelIndex &parent ) const
  {
  const SProperty *prop = _root;
  int size = 0;
  if(parent.isValid())
    {
    prop = (SProperty *)parent.internalPointer();
    }

  if(!prop)
    {
    return QModelIndex();
    }

  if(_options.hasFlag(EntitiesOnly))
    {
    const SEntity *ent = prop->castTo<SEntity>();
    xAssert(ent);

    prop = &ent->children;
    }

  const SPropertyContainer *container = prop->castTo<SPropertyContainer>();
  if(container)
    {
    SProperty *child = container->firstChild();
    while(child)
      {
      if(size == row)
        {
        return createIndex(row, column, child);
        }
      size++;
      child = child->nextSibling();
      }
    }

  return QModelIndex();
  }

QModelIndex SDatabaseModel::parent( const QModelIndex &child ) const
  {
  if(child.isValid())
    {
    SProperty *prop = (SProperty *)child.internalPointer();
    if(prop->parent() && prop->parent() != _root)
      {
      if(_options.hasFlag(EntitiesOnly))
        {
        SEntity *ent = prop->castTo<SEntity>();
        xAssert(ent);

        return createIndex(ent->parentEntity()->index(), 0, ent->parentEntity());
        }
      else
        {
        return createIndex(prop->parent()->index(), 0, prop->parent());
        }
      }
    }
  return QModelIndex();
  }

int SDatabaseModel::columnCount( const QModelIndex &parent ) const
  {
  const SProperty *prop = _root;
  if(parent.isValid())
    {
    prop = (SProperty *)parent.internalPointer();
    }

  if(_options.hasFlag(ShowValues) && prop)
    {
    xsize columns = 1;

    const SPropertyContainer *cont = prop->castTo<SPropertyContainer>();
    if(cont)
      {
      SProperty *child = cont->firstChild();
      while(child)
        {
        // this could maybe be improved, but we dont want to show the values for complex widgets...
        if(child->castTo<SPropertyContainer>())
          {
          columns = 2;
          break;
          }

        child = child->nextSibling();
        }
      }
    return columns;
    }
  return 1;
  }

QVariant SDatabaseModel::data( const QModelIndex &index, int role ) const
  {
  SProperty *prop = (SProperty *)index.internalPointer();
  if(prop && role == Qt::DisplayRole)
    {
    if(_options.hasFlag(ShowValues) && index.column() == 1)
      {
      SPropertyData data;
      _db->write(prop, data, SPropertyData::Ascii);
      return QString::fromUtf8(data.value());
      }
    else
      {
      return prop->name();
      }
    }
  return QVariant();
  }

bool SDatabaseModel::setData(const QModelIndex & index, const QVariant & value, int role)
  {
  return true;
  }

Qt::ItemFlags SDatabaseModel::flags(const QModelIndex &index) const
  {
  SProperty *prop = (SProperty *)index.internalPointer();
  if(prop && index.column() == 1)
    {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
  return QAbstractItemModel::flags(index);
  }

void SDatabaseModel::onTreeChange(int m, const SChange *c)
  {
  const SEntity::TreeChange *tC = c->castTo<SEntity::TreeChange>();
  if(tC)
    {
    if(tC->property() == _root && m&SChange::Forward && tC->after() == 0)
      {
      _root = 0;
      }
    emit layoutAboutToBeChanged();

    if(tC->after() == 0)
      {
      changePersistentIndex(createIndex(tC->index(), 0, (void*)tC->property()), QModelIndex());
      }

    emit layoutChanged();
    }

  const SProperty::NameChange *nameChange = c->castTo<SProperty::NameChange>();
  if(nameChange)
    {
    emit dataChanged(index(0,0), index(_root->children.size(),0));
    }
  }

void SDatabaseModel::actOnChanges()
  {
  }

void SDatabaseModel::setOptions(Options options)
  {
  emit layoutAboutToBeChanged();
  _options = options;
  emit layoutChanged();
  }

SDatabaseModel::Options SDatabaseModel::options() const
  {
  return _options;
  }

void SDatabaseModel::setRoot(SEntity *ent)
  {
  beginResetModel();
  if(_root)
    {
    _root->removeTreeObserver(this);
    }
  _root = ent;

  if(_root)
    {
    _root->addTreeObserver(this);
    }
  endResetModel();

  emit dataChanged(index(0, 0), index(_root->children.size(), 0));
  }

void SDatabaseModel::setDatabase(SDatabase *db, SEntity *root)
  {
  _db = db;

  if(root == 0)
    {
    root = db;
    }

  setRoot(root);
  }
