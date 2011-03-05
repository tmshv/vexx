#include "XEnvironment.h"
#include "XAbstractEnvironmentInterface.h"
#include "QDataStream"

XEnvironment::XEnvironment(XAbstractEnvironmentInterface *iface) :
    XInitProperty(environmentInterface, iface), _requestID(0)
  {
  environmentInterface()->setController(this);
  }

XEnvironment::ItemID XEnvironment::createItem(ItemType type)
  {
  Request item = requestSpecialItem(type, CreateItem, true);

  WriteLock lock(this);
  ItemID id;
  QDataStream stream(&_specials[CreateItem], QIODevice::ReadOnly);
  stream >> id;
  _specials.remove(CreateItem);
  _areas[id] = Area();
  return id;
  }

void XEnvironment::requestComplete(xuint32 requestID)
  {
  WriteLock lock(this);
  for(int i=0; i<_pendingRequests.size(); ++i)
    {
    if(_pendingRequests[i].requestID() == requestID)
      {
      _pendingRequests.removeAt(i);
      }
    }
  qDebug() << "Request Complete" << requestID;
  }

void XEnvironment::lock()
  {
  _lock.lock();
  }

void XEnvironment::unlock()
  {
  _lock.unlock();
  }

void XEnvironment::readLock() const
  {
  _lock.lock();
  }

void XEnvironment::readUnlock() const
  {
  _lock.unlock();
  }

QByteArray XEnvironment::getData(const Request &req, bool *correct) const
  {
  correct ? *correct = false : false;
    {
    ReadLock lock(this);
#warning switch on type and stream to byte arr etc...
    }
  return QByteArray();
  }

void XEnvironment::setData(const Request &req, const QByteArray &arr)
  {
  WriteLock lock(this);
#warning switch on type etc...
  }

void XEnvironment::syncData(const Request &req)
  {
  environmentInterface()->syncItem( req );
  }

void XEnvironment::requestItem( Request &request, bool block )
  {
  qDebug() << "Request Item" << request.type() << request.ID() << request.subType();

  WriteLock lock(this);
  request.setRequestID(_requestID++);
  _pendingRequests << request;
  environmentInterface()->requestItem( request );

  if(block)
    {
    while(hasUncompleteRequest(request.requestID()))
      {
      // now refresh the data interface until it has our item
      environmentInterface()->poll();
      }
    }
  }

bool XEnvironment::hasUncompleteRequest(xuint32 id) const
  {
  ReadLock lock(this);
  foreach(const Request &req, _pendingRequests)
    {
    if(req.requestID() == id)
      {
      return true;
      }
    }
  return false;
  }

XEnvironment::Request XEnvironment::requestSpecialItem( ItemID id, SpecialType type, bool block )
  {
  Request request(Special, id, type);
  requestItem( request, block );
  return request;
  }
