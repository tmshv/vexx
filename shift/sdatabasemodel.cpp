#include "sdatabasemodel.h"
#include "sentity.h"
#include "sdatabase.h"
#include "QPushButton"
#include "QStyleOptionViewItem"

#define SDataModelProfileFunction XProfileFunction(ShiftDataModelProfileScope)
#define SDataModelProfileScopedBlock(mess) XProfileScopedBlock(ShiftDataModelProfileScope, mess)

SDatabaseDelegate::SDatabaseDelegate(QObject *parent) : QItemDelegate(parent), _currentWidget(0)
  {
  }

QWidget *SDatabaseDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
  {
  _currentIndex = index;
  if(index.isValid())
    {
    SProperty *prop = (SProperty *)index.internalPointer();
    _currentWidget = _ui.createControlWidget(prop, parent);
    if(_currentWidget)
      {
      connect(_currentWidget, SIGNAL(destroyed(QObject *)), this, SLOT(currentItemDestroyed()));
      emit ((SDatabaseDelegate*)this)->sizeHintChanged(_currentIndex);
      }
    return _currentWidget;
    }
  return 0;
  }

void SDatabaseDelegate::setEditorData(QWidget *, const QModelIndex &) const
  {
  }

void SDatabaseDelegate::setModelData(QWidget *, QAbstractItemModel *, const QModelIndex &) const
  {
  }

QSize SDatabaseDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
  {
  SDataModelProfileFunction
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

  if(_root.isValid())
    {
    _root->addTreeObserver(this);
    }
  }

SDatabaseModel::~SDatabaseModel()
  {
  if(_root.isValid())
    {
    _root->removeTreeObserver(this);
    }
  }

int SDatabaseModel::rowCount( const QModelIndex &parent ) const
  {
  SDataModelProfileFunction
  const SProperty *prop = _root;
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
    return container->size();
    }

  return 0;
  }

QModelIndex SDatabaseModel::index( int row, int column, const QModelIndex &parent ) const
  {
  SDataModelProfileFunction
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
  SDataModelProfileFunction
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
  SDataModelProfileFunction
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
        SPropertyVariantInterface *interface = child->interface<SPropertyVariantInterface>();
        if(interface)
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
  SDataModelProfileFunction
  SProperty *prop = (SProperty *)index.internalPointer();
  if(prop && role == Qt::DisplayRole)
    {
    if(_options.hasFlag(ShowValues) && index.column() == 1)
      {
      SPropertyVariantInterface *interface = prop->interface<SPropertyVariantInterface>();
      if(interface)
        {
        return interface->asVariant(prop);
        }
      return QVariant();;
      }
    else
      {
      return prop->name();
      }
    }
  return QVariant();
  }

bool SDatabaseModel::setData(const QModelIndex &, const QVariant &, int)
  {
  return true;
  }

QVariant SDatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
  if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
    if(section == 0)
      {
      return "Property";
      }
    else if(section == 1)
      {
      return "Value";
      }
    }
  return QVariant();
  }

Qt::ItemFlags SDatabaseModel::flags(const QModelIndex &index) const
  {
  SDataModelProfileFunction
  SProperty *prop = (SProperty *)index.internalPointer();
  if(prop && index.column() == 1)
    {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
  return QAbstractItemModel::flags(index);
  }

void SDatabaseModel::onTreeChange(const SChange *c)
  {
  const SEntity::TreeChange *tC = c->castTo<SEntity::TreeChange>();
  if(tC)
    {
    if(tC->property() == _root && tC->after() == 0)
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
