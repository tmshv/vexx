#include "sdatabasemodel.h"
#include "sentity.h"
#include "sdatabase.h"
#include "QPushButton"
#include "QStyleOptionViewItem"

Q_DECLARE_METATYPE(QModelIndex)

#define SDataModelProfileFunction XProfileFunction(ShiftDataModelProfileScope)
#define SDataModelProfileScopedBlock(mess) XProfileScopedBlock(ShiftDataModelProfileScope, mess)

SDatabaseDelegate::SDatabaseDelegate(QObject *parent) : QItemDelegate(parent), _currentWidget(0)
  {
  }

QWidget *SDatabaseDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
  {
  if(index.isValid())
    {
    SProperty *prop = (SProperty *)index.internalPointer();
    _currentWidget = _ui.createControlWidget(prop, parent);
    if(_currentWidget)
      {
      _currentIndex = index;
      connect(_currentWidget, SIGNAL(destroyed(QObject *)), this, SLOT(currentItemDestroyed()));
      emit ((SDatabaseDelegate*)this)->sizeHintChanged(_currentIndex);
      }
    else
      {
      _currentIndex = QModelIndex();
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

  QHash<int, QByteArray> roles;
  roles[Qt::DisplayRole] = "name";
  roles[PropertyPositionRole] = "propertyPosition";
  roles[PropertyColourRole] = "propertyColour";
  roles[PropertyInputRole] = "propertyInput";
  setRoleNames(roles);
  }

SDatabaseModel::~SDatabaseModel()
  {
  if(_root.isValid())
    {
    _root->removeTreeObserver(this);
    }
  }

bool SDatabaseModel::isEqual(const QModelIndex &a, const QModelIndex &b) const
  {
  return a.internalPointer() == b.internalPointer();
  }

bool SDatabaseModel::isValid(const QModelIndex &a) const
  {
  return a.isValid();
  }

int SDatabaseModel::rowIndex(const QModelIndex &i) const
  {
  return i.row();
  }

int SDatabaseModel::columnIndex(const QModelIndex &i) const
  {
  return i.column();
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
  if(prop)
    {
    if(role == Qt::DisplayRole)
      {
      if(_options.hasFlag(ShowValues) && index.column() == 1)
        {
        SPropertyVariantInterface *interface = prop->interface<SPropertyVariantInterface>();
        if(interface)
          {
          return interface->asVariant(prop);
          }
        return QVariant();
        }
      else
        {
        QString name = prop->name();
        xAssert(!name.isEmpty());
        return name;
        }
      }
    else if(role == PropertyPositionRole)
      {
      SPropertyPositionInterface *interface = prop->interface<SPropertyPositionInterface>();
      if(interface)
        {
        return toQt(interface->position(prop));
        }
      return QVector3D();
      }
    else if(role == PropertyColourRole)
      {
      SPropertyColourInterface *interface = prop->interface<SPropertyColourInterface>();
      if(interface)
        {
        return interface->colour(prop).toLDRColour();
        }
      return QColor();
      }
    else if(role == PropertyInputRole)
      {
      SProperty *inp = prop->input();
      if(inp)
        {
        return QVariant::fromValue(createIndex(inp->index(), 0, inp));
        }
      }
    }
  return QVariant();
  }

bool SDatabaseModel::setData(const QModelIndex &index, const QVariant &val, int role)
  {
  SDataModelProfileFunction
  SProperty *prop = (SProperty *)index.internalPointer();
  if(prop)
    {
    if(role == Qt::DisplayRole)
      {
      if(_options.hasFlag(ShowValues) && index.column() == 1)
        {
        SPropertyVariantInterface *interface = prop->interface<SPropertyVariantInterface>();
        if(interface)
          {
          interface->setVariant(prop, val);
          return true;
          }
        }
      else
        {
        prop->setName(val.toString());
        return true;
        }
      }
    else if(role == PropertyPositionRole)
      {
      SPropertyPositionInterface *interface = prop->interface<SPropertyPositionInterface>();
      if(interface)
        {;
        QVector3D vec = val.value<QVector3D>();
        interface->setPosition(prop, XVector3D(vec.x(), vec.y(), vec.z()));
        return true;
        }
      }
    }
  return false;
  }

bool SDatabaseModel::setData(const QModelIndex &index, const QString &role, const QVariant &value)
  {
  QHash<int, QByteArray> roles = roleNames();

  foreach(int i, roles.keys())
    {
    QByteArray name = roles.value(i);

    if(role == name)
      {
      return setData(index, value, i);
      }
    }

  return false;
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
  if(prop && index.column() < 2)
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

      const SPropertyContainer *parent = tC->before();
      xAssert(parent);

      xsize i = tC->index();
      emit beginRemoveRows(createIndex(parent->index(), 0, (void *)parent), i, i);
      emit endRemoveRows();
      }
    else
      {
      const SPropertyContainer *parent = tC->after();
      xAssert(parent);

      xsize i = tC->index();
      emit beginInsertRows(createIndex(parent->index(), 0, (void *)parent), i+1, i+1);
      emit endInsertRows();
      }

    emit layoutChanged();
    }

  const SProperty::NameChange *nameChange = c->castTo<SProperty::NameChange>();
  if(nameChange)
    {
    const SProperty *prop = nameChange->property();
    QModelIndex ind = createIndex(prop->index(), 0, (void *)prop);
    emit dataChanged(ind, ind);
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
