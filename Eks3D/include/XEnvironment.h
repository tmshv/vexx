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

  enum ItemSubType
    {
    InfoSubType,

    LastSubType
    };
  typedef XEnvironmentArea Area;
  typedef XEnvironmentID ItemID;

  class EKS3D_EXPORT Container
    {
  XProperties:
    class EKS3D_EXPORT Item
      {
    XProperties:
      XProperty(xuint16, type, setType);
      XProperty(ItemID, ID, setID);

      friend QDataStream &operator<<(QDataStream &s, const Item &c) { return s << c._type << c._ID; }
      friend QDataStream &operator>>(QDataStream &s, Item &c) { return s >> c._type >> c._ID; }
      };

    XRefProperty(QList <Item>, items);
    XProperty(QString, name, setName);
    XROProperty(ItemID, parentID);
    XROProperty(ItemID, ID);

    Container(ItemID id);
    void reparent(Container *oldParent, Container *newParent);

    xsize indexOf(const Container *c) const { xAssertFail(); return -1;/*int in = _items.indexOf(c->ID()); return in == -1 ? X_SIZE_SENTINEL : in;*/ }

    friend EKS3D_EXPORT QDataStream &operator<<(QDataStream &stream, const Container &c);
    friend EKS3D_EXPORT QDataStream &operator>>(QDataStream &stream, Container &c);
    };

  class EKS3D_EXPORT TextureInfo
    {
  XProperties:
    XProperty(QString, name, setName);
    XROProperty(ItemID, parentID);
    XROProperty(ItemID, ID);

    TextureInfo(ItemID id);
    void reparent(Container *oldParent, Container *newParent);

    friend EKS3D_EXPORT QDataStream &operator<<(QDataStream &stream, const TextureInfo &c);
    friend EKS3D_EXPORT QDataStream &operator>>(QDataStream &stream, TextureInfo &c);
    };


  typedef XHash<ItemID, QByteArray*> ByteArrayHash;
  typedef XHash<ItemID, Area*> AreaHash;
  typedef XHash<ItemID, Container*> ContainerHash;
  typedef XHash<ItemID, XGeometry*> GeomtryHash;
  typedef XHash<ItemID, XShader*> ShaderHash;
  typedef XHash<ItemID, XTexture*> TextureHash;
  typedef XHash<ItemID, TextureInfo*> TextureInfoHash;
  typedef XEnvironmentRequest Request;

  enum SpecialIdentifier
    {
    CreateItem
    };

XProperties:
  XROProperty(XAbstractEnvironmentInterface *, environmentInterface);

public:
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
  TextureInfo *textureInfo(ItemID id) { return _textureInfos.value(id, 0); }
  Area *area(ItemID id) { return _areas.value(id, 0); }
  XGeometry *mesh(ItemID id) { return _meshes.value(id, 0); }

  void requestItem( Request &, bool block=false );

  ItemID createItem(ItemType type);

private:
  Request requestSpecialItem( ItemID id, SpecialIdentifier type, bool block );

  ByteArrayHash _specials;
  ContainerHash _containers;
  TextureInfoHash _textureInfos;
  ShaderHash _shaders;
  GeomtryHash _meshes;
  AreaHash _areas;
  XList<Request> _pendingRequests;

  Listener *_listener;

  xuint32 _requestID;
  };

#endif // XENVIRONMENT_H
