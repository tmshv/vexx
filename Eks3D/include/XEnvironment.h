#ifndef XENVIRONMENT_H
#define XENVIRONMENT_H

#include "XProperty"
#include "QMutex"
#include "XHash"
#include "XTexture.h"
#include "XShader.h"
#include "XEnvironmentArea.h"
#include "XEnvironmentRequest.h"

class XRenderer;
class XAbstractEnvironmentInterface;
class XPerspectiveCamera;

class EKS3D_EXPORT XEnvironment
  {
public:
  class Container
    {
  public:
    };

  typedef XEnvironmentArea Area;
  typedef XEnvironmentID ItemID;

  typedef XHash<ItemID, QByteArray> ByteArrayHash;
  typedef XHash<ItemID, Area> AreaHash;
  typedef XHash<ItemID, Container> ContainerHash;
  typedef XHash<ItemID, XGeometry> GeomtryHash;
  typedef XHash<ItemID, XShader> ShaderHash;
  typedef XHash<ItemID, XTexture> TextureHash;
  typedef XEnvironmentRequest Request;

  enum SpecialType
    {
    CreateItem
    };

XProperties:
  XROProperty(XAbstractEnvironmentInterface *, environmentInterface);

public:
  enum ItemType
  {
    Special,
    Container,
    Texture,
    Shader,
    Mesh,
    World,
    LastType
  };

  class WriteLock
    {
  public:
    WriteLock(XEnvironment* locked) : _locked(locked)
      {
      _locked->lock();
      }
    ~WriteLock()
      {
      _locked->unlock();
      }
  private:
    XEnvironment *_locked;
    };

  class ReadLock
    {
  public:
    ReadLock(const XEnvironment* locked) : _locked(locked)
      {
      _locked->readLock();
      }
    ~ReadLock()
      {
      _locked->readUnlock();
      }
  private:
    const XEnvironment *_locked;
    };

  XEnvironment(XAbstractEnvironmentInterface *);

  void requestComplete(xuint32 requestID);

  void lock();
  void unlock();

  void readLock() const;
  void readUnlock() const;

  bool dataExists(const Request &req) const;
  QByteArray getData(const Request &req, bool *correct = 0) const;
  void setData(const Request &req, const QByteArray &);
  void syncData(const Request &req);

  const Area *area(ItemID id) const;
  const XGeometry *mesh(ItemID id) const;

  void requestItem( Request &, bool block=false );

  ItemID createItem(ItemType type);

private:
  Request requestSpecialItem( ItemID id, SpecialType type, bool block );
  bool hasUncompleteRequest(xuint32 id) const;

  mutable QMutex _lock;

  ByteArrayHash _specials;
  ContainerHash _containers;
  TextureHash _textures;
  ShaderHash _shaders;
  GeomtryHash _meshes;
  AreaHash _areas;
  XList<Request> _pendingRequests;

  xuint32 _requestID;
  };

#endif // XENVIRONMENT_H
