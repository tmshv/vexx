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
  typedef XEnvironmentArea Area;
  typedef XEnvironmentID ItemID;

  class EKS3D_EXPORT Container
    {
  XProperties:
    XRefProperty(QList <ItemID>, items);
    XProperty(QString, name, setName);
    XROProperty(ItemID, parentID);
    XROProperty(ItemID, ID);

    Container(ItemID id);
    void reparent(Container *oldParent, Container *newParent);

    xsize indexOf(const Container *c) const { int in = _items.indexOf(c->ID()); return in == -1 ? X_SIZE_SENTINEL : in; }

    friend EKS3D_EXPORT QDataStream &operator<<(QDataStream &stream, const Container &c);
    friend EKS3D_EXPORT QDataStream &operator>>(QDataStream &stream, Container &c);
    };


  typedef XHash<ItemID, QByteArray*> ByteArrayHash;
  typedef XHash<ItemID, Area*> AreaHash;
  typedef XHash<ItemID, Container*> ContainerHash;
  typedef XHash<ItemID, XGeometry*> GeomtryHash;
  typedef XHash<ItemID, XShader*> ShaderHash;
  typedef XHash<ItemID, XTexture*> TextureHash;
  typedef XEnvironmentRequest Request;

  enum SpecialIdentifier
    {
    CreateItem
    };

XProperties:
  XROProperty(XAbstractEnvironmentInterface *, environmentInterface);

public:
  enum ItemType
  {
    SpecialType,
    ContainerType,
    TextureType,
    ShaderType,
    MeshType,
    WorldType,

    LastType
  };

  class EKS3D_EXPORT Listener
    {
  public:
    virtual void onRequestComplete(const Request &request) = 0;
    };

  XEnvironment(XAbstractEnvironmentInterface *i, Listener *l);


  void requestComplete(xuint32 requestID);

  bool dataExists(const Request &req) const;
  QByteArray getData(const Request &req, bool *correct = 0) const;
  void setData(const Request &req, const QByteArray &);
  void syncData(const Request &req);

  bool hasUncompleteRequest(const Request &r) const;
  bool hasUncompleteRequestForSameData(const Request &req) const;

  const Container *container(ItemID id) const { return _containers.value(id, 0); }
  const Area *area(ItemID id) const { return _areas.value(id, 0); }
  const XGeometry *mesh(ItemID id) const { return _meshes.value(id, 0); }

  Container *container(ItemID id) { return _containers.value(id, 0); }
  Area *area(ItemID id) { return _areas.value(id, 0); }
  XGeometry *mesh(ItemID id) { return _meshes.value(id, 0); }

  void requestItem( Request &, bool block=false );

  ItemID createItem(ItemType type);

private:
  Request requestSpecialItem( ItemID id, SpecialIdentifier type, bool block );

  ByteArrayHash _specials;
  ContainerHash _containers;
  TextureHash _textures;
  ShaderHash _shaders;
  GeomtryHash _meshes;
  AreaHash _areas;
  XList<Request> _pendingRequests;

  Listener *_listener;

  xuint32 _requestID;
  };

#endif // XENVIRONMENT_H
