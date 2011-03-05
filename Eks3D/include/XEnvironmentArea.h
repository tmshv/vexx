#ifndef XENVIRONMENTITEM_H
#define XENVIRONMENTITEM_H

#include "XGlobal"
#include "XCuboid.h"
#include "XVector"
#include "XMap"

class EKS3D_EXPORT XEnvironmentArea
  {
public:
  typedef XEnvironmentID ItemID;
  typedef xuint16 SubType;

  enum CommonSubType
    {
    Item,
    CubeMap,
    Name,
    VisBoxBegin = 100,
    Maximum,
    };

  class MeshPair
    {
  properties:
    typedef ItemID MeshID;
    XProperty(MeshID, mesh, setMesh);
    XRefProperty(XTransform, transform);

    friend EKS3D_EXPORT QDataStream &operator<<(QDataStream &stream, const MeshPair &itemRequest);
    friend EKS3D_EXPORT QDataStream &operator>>(QDataStream &stream, MeshPair &itemRequest);
    };
  typedef XVector <MeshPair> MeshPairList;

  class ShadingGroup
    {
  properties:
    typedef ItemID ShaderID;
    XRORefProperty(MeshPairList, meshes);
    XProperty(ShaderID, shader, setShader);

    friend EKS3D_EXPORT QDataStream &operator<<(QDataStream &stream, const ShadingGroup &itemRequest);
    friend EKS3D_EXPORT QDataStream &operator>>(QDataStream &stream, ShadingGroup &itemRequest);
    };

properties:
  typedef XVector<ShadingGroup> ShadingGroupList;
  typedef XVector<ItemID> VisBox;
  typedef XMap<ItemID, VisBox> VisBoxHash;

  XRefProperty(XCuboid, bounds);
  XRefProperty(XVector<ItemID>, childAreas);
  XRefProperty(ShadingGroupList, shadingGroups);
  XProperty(xuint32, visBoxWidth, setVisBoxWidth);
  XProperty(xuint32, visBoxHeight, setVisBoxHeight);
  XProperty(xuint32, visBoxDepth, setVisBoxDepths);

  XRefProperty(VisBox, permanentVisBox);
  XRefProperty(VisBoxHash, spatialVisBoxes);

public:
  XEnvironmentArea();

  void save(QDataStream &stream) const;
  void restore(QDataStream &stream);

  enum
    {
    PermanentVisBox = 0
    };
  xuint32 getVisBoxID(xuint32 w, xuint32 h, xuint32 d) const;

  void saveVisBox(QDataStream &stream, xuint32 id) const;
  void restoreVisBox(QDataStream &stream, xuint32 id);
  };

#endif // XENVIRONMENTITEM_H
