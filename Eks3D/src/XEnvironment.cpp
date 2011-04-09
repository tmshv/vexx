#include "XEnvironment.h"
#include "XAbstractEnvironmentInterface.h"
#include "QDataStream"

XEnvironment::XEnvironment(XAbstractEnvironmentInterface *iface, Listener *l) :
    XInitProperty(environmentInterface, iface), _listener(l), _requestID(0)
  {
  environmentInterface()->setController(this);
  }

XEnvironment::ItemID XEnvironment::createItem(ItemType type)
  {
  Request item = requestSpecialItem(type, CreateItem, true);

  ItemID id;
  xAssert(_specials[CreateItem]);
  QDataStream stream(_specials[CreateItem], QIODevice::ReadOnly);
  stream >> id;
  _specials.remove(CreateItem);
  return id;
  }

void XEnvironment::requestComplete(xuint32 requestID)
  {
  Request r;
    {
    for(int i=0; i<_pendingRequests.size(); ++i)
      {
      if(_pendingRequests[i].requestID() == requestID)
        {
        r = _pendingRequests[i];
        _pendingRequests.removeAt(i);
        }
      }
    }

  xAssert(_listener);
  _listener->onRequestComplete(r);
  qDebug() << "Request Complete" << requestID;
  }

QByteArray XEnvironment::getData(const Request &req, bool *correct) const
  {
  correct ? *correct = false : false;

  QByteArray arr;
  if(req.type() == ContainerType && _containers.contains(req.ID()))
    {
    QDataStream str(&arr, QIODevice::WriteOnly);
    str << *_containers[req.ID()];

    correct ? *correct = true : true;
    }
  else if(req.type() == SpecialType && _specials.contains(req.ID()))
    {
    arr = *_specials[req.ID()];

    correct ? *correct = true : true;
    }
  else
    {
    xAssertFail();
    }

  return arr;
  }

void XEnvironment::setData(const Request &req, const QByteArray &arr)
  {
  if(req.type() == ContainerType)
    {
    Container *&ctr = _containers[req.ID()];
    if(!ctr)
      {
      ctr = new Container(req.ID());
      }

    if(arr.length())
      {
      // cast because we are not going to change to contents due to ReadOnly, but IODevice expects a non-const ptr.
      QDataStream str((QByteArray*)&arr, QIODevice::ReadOnly);

      str >> *ctr;
      }
    }
  else if(req.type() == SpecialType)
    {
    QByteArray *&spe = _specials[req.subType()];
    if(!spe)
      {
      spe = new QByteArray;
      }

    *spe = arr;
    }
  else
    {
    xAssertFail();
    }
  }

void XEnvironment::syncData(const Request &req)
  {
  environmentInterface()->syncItem( req );
  }

void XEnvironment::requestItem(Request &request, bool block)
  {
  qDebug() << "Request Item" << request.type() << request.ID() << request.subType();

  if(!hasUncompleteRequestForSameData(request))
    {
    request.setRequestID(_requestID++);
    _pendingRequests << request;
    environmentInterface()->requestItem( request );
    }

  if(block)
    {
    while(hasUncompleteRequest(request))
      {
      // now refresh the data interface until it has our item
      environmentInterface()->poll();
      }
    }
  }

bool XEnvironment::hasUncompleteRequest(const Request &r) const
  {
  foreach(const Request &req, _pendingRequests)
    {
    if(req.requestID() == r.requestID())
      {
      return true;
      }
    }
  return false;
  }

bool XEnvironment::hasUncompleteRequestForSameData(const Request &req) const
  {
  foreach(const Request &r, _pendingRequests)
    {
    if(req.type() == r.type() && req.ID() == r.ID() && req.subType() == r.subType())
      {
      return true;
      }
    }
  return false;
  }

XEnvironment::Request XEnvironment::requestSpecialItem(ItemID id, SpecialIdentifier type, bool block)
  {
  Request request(SpecialType, id, type);
  requestItem( request, block );
  return request;
  }

XEnvironment::Container::Container(ItemID id) : _parentID(0), _ID(id)
  {
  }

void XEnvironment::Container::reparent(Container *oldParent, Container *parent)
  {
  xAssert(oldParent);
  xAssert(parent);
  oldParent->items().removeAll(ID());
  _parentID = parent->ID();
  parent->items() << ID();
  }

QDataStream &operator<<(QDataStream &stream, const XEnvironment::Container &c)
  {
  return stream << c._items << c._name << c._parentID;
  }

QDataStream &operator>>(QDataStream &stream, XEnvironment::Container &c)
  {
  return stream >> c._items >> c._name >> c._parentID;
  }
