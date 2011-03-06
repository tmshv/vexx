#ifndef XENVIRONMENTRENDERER_H
#define XENVIRONMENTRENDERER_H

#include "X3DGlobal.h"
#include "XObject"
#include "XProperty"
#include "XList"
#include "XShape.h"
#include "XVector3D"
#include "XTransform.h"
#include "XEnvironmentArea.h"
#include "XEnvironmentRequest.h"
#include "XFrustum.h"
#include "QBitArray"

class XEnvironmentViewer;
class XEnvironment;
class XRenderer;

class EKS3D_EXPORT XEnvironmentRenderer : public XObject
  {
public:
  typedef XEnvironmentArea Area;
  typedef XEnvironmentID ItemID;
  typedef XEnvironmentArea::SubType SubType;
  typedef XHash<ItemID,Area> AreaHash;
  typedef XEnvironmentRequest Request;

  class EKS3D_EXPORT RayCastResult;
  class EKS3D_EXPORT RayCastResults : public XList<RayCastResult>
    {
  public:
    bool hasResults() const;
    quint32 maximumDepth() const;
    RayCastResults leafResults() const;
    RayCastResult closestResult(const XVector3D &) const;
    };

  class EKS3D_EXPORT RayCastResult
    {
  XProperties:
    XROProperty( XTransform, worldTransform );
    XROProperty( ItemID, ID );
    XRORefProperty( RayCastResults, results );
    XRORefProperty( XList<xuint32>, parentTree );
    XROProperty( xuint32, index );

  public:
    class Hit
      {
      XROProperty( XVector3D, localPosition );
      XROProperty( unsigned int, triangleIndex );
      XROProperty( const XGeometry *, geometry );
    public:
      Hit( const XGeometry *geometry, const XVector3D &lP, unsigned int indices );
      };
    typedef XList<Hit> HitList;

  private:
    XRORefProperty( HitList, hits );

  public:
    RayCastResult();
    RayCastResult( ItemID id, const XTransform &tx, const RayCastResults &res, const XList<xuint32> &parentTree, xuint32 index, const HitList &hits=HitList() );
    bool isValid() const;
    };

XProperties:
  XROProperty(XRenderer *, renderer);
  XProperty(XEnvironment *, environment, setEnvironment);
  XProperty(XEnvironmentViewer *, viewer, setViewer);

public:
  XEnvironmentRenderer(XEnvironment *env, XEnvironmentViewer *viewer, XRenderer *renderer);

  static void initiateEnvironment();

  void render();

  RayCastResults rayCast( QPoint point, bool exact=true );
  RayCastResults rayCast( const XLine &line, bool exact=false );

private:
  bool exactRayCast( ItemID id, const XLine &ray, const XTransform &worldTransform, RayCastResult::HitList &hits );
  void rayCastPrivate( ItemID id, const XLine &ray, const XTransform &world, RayCastResults &, bool exact, xuint32 index, XList<xuint32> parentTree );

  void numberOfLightsChanged(xuint16 light);
  void lightChanged(xuint16 light);
  QBitArray _lightsNeedRefresh;

  XShape _cubeMap;
  bool _needCubeMap;

  quint64 _rootItem;
  };

#endif // XENVIRONMENTRENDERER_H
