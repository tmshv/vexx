#include "EnvironmentEntity.h"

#if 0

#include "sevent.h"
#include "XVector3D"
#include "XQuaternion"
#include "sproperty.h"

EnvironmentEntity::EnvironmentEntity(XEnvironment *environment, XEnvironment::ItemID item) : _environment(environment), _item(item)
  {
  }

void EnvironmentEntity::setItem(XEnvironment::ItemID in, XEnvironment::ItemID parent, xuint32 childIndex)
  {
  XVariant oldName = getData(Name, SProperty::Value);
  XVariant oldID = getData(ID, SProperty::Value);
  XVariant oldPosition = getData(Position, SProperty::Value);
  XVariant oldOrientation = getData(Orientation, SProperty::Value);

  _item = in;
  _parent = parent;
  _index = childIndex;

  dataChanged(SDataEvent(this, Name, SProperty::Value, oldName, getData(Name, SProperty::Value)));
  dataChanged(SDataEvent(this, ID, SProperty::Value, oldID, getData(ID, SProperty::Value)));
  dataChanged(SDataEvent(this, Position, SProperty::Value, oldPosition, getData(ID, SProperty::Value)));
  dataChanged(SDataEvent(this, Orientation, SProperty::Value, oldOrientation, getData(ID, SProperty::Value)));
  }

XVariant EnvironmentEntity::getData( SPropertyID id, SAttributeID attribute ) const
  {
  if(id == SEntity::Name)
    {
    if(attribute == SProperty::Value)
      {
      bool correct = false;
      QByteArray nameData = _environment->getItemData(_item, XEnvironmentItem::Name, &correct);
      if(!correct)
        {
        // blocking for the item to be recieved
        _environment->blockForItem(_item);
        nameData = _environment->getItemData(_item, XEnvironmentItem::Name, &correct);
        }

      QDataStream str(&nameData, QIODevice::ReadOnly);
      QString name;
      str >> name;
      return name;
      }
    }
  else if(id == ID)
    {
    if(attribute == SProperty::Value)
      {
      return _item;
      }
    else if(attribute == SProperty::Name)
      {
      return QString("ID");
      }
    else if(attribute == SProperty::ReadOnly)
      {
      return true;
      }
    }
  else if(id == Position)
    {
    if(attribute == SProperty::Value)
      {
      XEnvironmentItem* item = _environment->beginItemLockBlocking(_parent);
      if(item)
        {
        xAssert(_index < (xuint32)item->children().size());
        const XEnvironmentItem::Child &child = item->children()[_index];
        xAssert(child.ID() == _item);

        return child.transform().translation();
        }
      }
    else if(attribute == SProperty::Name)
      {
      return QString("Position");
      }
    }
  else if(id == Orientation)
    {
    if(attribute == SProperty::Value)
      {
      return XQuaternion();
      }
    else if(attribute == SProperty::Name)
      {
      return QString("Orientation");
      }
    }
  return SEntity::getData(id, attribute);
  }

bool EnvironmentEntity::setData( SPropertyID id, SAttributeID attribute, const XVariant &value )
  {
  if(id == SEntity::Name && attribute == SProperty::Value && value.is<QString>())
    {
    QString name = value.toString();
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);
    str << name;
    // todo: maybe check they arent the same before setting?
    _environment->setItemData( _item, XEnvironmentItem::Name, arr );
    _environment->syncItemData( _item, XEnvironmentItem::Name );
    return true;
    }
  else if(id == Position && attribute == SProperty::Value && value.is<XVector3D>())
    {
    XEnvironmentItem* item = _environment->beginItemLockBlocking(_parent);
    if(item)
      {
      xAssert(_index < (xuint32)item->children().size());
      XEnvironmentItem::Child &child = item->children()[_index];
      xAssert(child.ID() == _item);

      child.transform().setTranslation(value.toVector3D());
      _environment->syncItem(_parent, XEnvironmentItem::Renderable);
      qDebug() << getData(id, attribute).toVector3D();
      return true;
      }
    }
  return false;
  }
#endif
