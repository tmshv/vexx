#include "XEnvironmentRenderer.h"
#include "XEnvironmentViewer.h"
#include "XAbstractEnvironmentInterface.h"
#include "XEnvironment.h"
#include "XFrustum.h"
#include "XTexture.h"
#include "XModeller.h"
#include "XRenderer.h"
#include "XShader.h"
#include "XLine.h"
#include "XShape.h"
#include "XLightManager.h"

//#define X_ENVIRONMENT_DRAW_BOUNDS

#if 0

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ray Cast Classes
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool XEnvironmentRenderer::RayCastResults::hasResults() const
  {
  return size() > 0;
  }

quint32 XEnvironmentRenderer::RayCastResults::maximumDepth() const
  {
  qint32 max = -1;
  foreach(const XEnvironmentRenderer::RayCastResult &result, *this)
    {
    max = qMax(max, (qint32)result.results().maximumDepth());
    }
  return max + 1;
  }

XEnvironmentRenderer::RayCastResults XEnvironmentRenderer::RayCastResults::leafResults() const
  {
  RayCastResults results;
  foreach(const XEnvironmentRenderer::RayCastResult &result, *this)
    {
    if(result.results().size())
      {
      results << result.results().leafResults();
      }
    else
      {
      results << result;
      }
    }
  return results;
  }

XEnvironmentRenderer::RayCastResult XEnvironmentRenderer::RayCastResults::closestResult(const XVector3D &point) const
  {
  XEnvironmentRenderer::RayCastResult ret;
  XEnvironmentRenderer::RayCastResults leaves(leafResults());
  float distance(HUGE_VAL);

  foreach(const RayCastResult &leaf, leaves)
    {
    float distanceTemp((point-leaf.worldTransform().translation()).lengthSquared());
    if( distance > distanceTemp )
      {
      ret = leaf;
      distance = distanceTemp;
      }
    }

  return ret;
  }

XEnvironmentRenderer::RayCastResult::Hit::Hit( const XGeometry *geometry,
                                       const XVector3D &lP,
                                       unsigned int index )
    : _localPosition(lP), _triangleIndex(index), _geometry(geometry)
  {
  }

XEnvironmentRenderer::RayCastResult::RayCastResult() : _ID(-1)
  {
  }

XEnvironmentRenderer::RayCastResult::RayCastResult( ItemID id,
               const XTransform &tx,
               const RayCastResults &res,
               const XList<xuint32> &parentTree,
               xuint32 index,
               const HitList &hits )
    : _worldTransform(tx),
      _ID(id),
      _results(res),
      _parentTree(parentTree),
      _index(index),
      _hits(hits)
  {
  }

void XEnvironmentRenderer::initiateEnvironment()
  {
  qRegisterMetaTypeStreamOperators<XTexture>( "XTexture" );
  }

bool XEnvironmentRenderer::RayCastResult::isValid() const
  {
  return (qint32)ID() != -1;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////
// XEnvironment
////////////////////////////////////////////////////////////////////////////////////////////////////////

XEnvironmentRenderer::XEnvironmentRenderer(XEnvironment *env, XEnvironmentViewer *viewer, XRenderer *ren)
    : XInitProperty(renderer, ren), _environment(env), _viewer(viewer), _needCubeMap(true), _rootItem(0)
  {
  initiateEnvironment();

  // set up
  _cubeMap.setRenderFlags( XRenderer::DepthTest );
  // draw a cube for the cubemap
    {
      float size = _viewer->farClipPlane();
    XModeller mod( &_cubeMap.geometry() );
    mod.drawCube( XVector3D( size, 0, 0 ), XVector3D( 0, size, 0 ), XVector3D( 0, 0, size ), 0.005, 0.005 );
    }
  _cubeMap.setShader( XShader( XShader::AmbientShader ) );

  X_CONNECT(XLightManager::instance(), lightChanged, this, lightChanged);
  }

void XEnvironmentRenderer::render()
  {
  xAssert(0);
#if 0
  environment()->environmentInterface()->poll();

  if(!environment()->itemExists(_rootItem, XEnvironmentItem::Renderable))
    {
    ItemRequest req( _rootItem, XEnvironmentItem::Renderable );
    environment()->requestItem( req );
    }

  drawItem(environment()->beginItemLockBlocking(_rootItem), XTransform(), true);
  environment()->endItemLock(_rootItem);

  if(_needCubeMap)
    {
    bool correct;
    QByteArray arr = environment()->getItemData( _rootItem, XEnvironmentItem::CubeMap, &correct );
    if( correct )
      {
      QDataStream str( &arr, QIODevice::ReadWrite );
      QImage im;
      str >> im;
      _needCubeMap = false;
      _cubeMap.shader()->getVariable("ambientTexture")->setValue(im);
      }
    }
  // draw cubemap
  renderer()->pushTransform( XTransform( viewer()->position() ) );
  renderer()->drawShape( _cubeMap );
  renderer()->popTransform();
#endif
  }

XEnvironmentRenderer::RayCastResults XEnvironmentRenderer::rayCast( QPoint point, bool exact )
  {
  RayCastResults result;
  XLine l(viewer()->position(), viewer()->screenToWorld(point));
  rayCastPrivate( _rootItem, l, XTransform(), result, exact, X_UINT32_SENTINEL, XList<xuint32>() );
  return result;
  }

XEnvironmentRenderer::RayCastResults XEnvironmentRenderer::rayCast( const XLine &ray, bool exact )
  {
  RayCastResults result;
  rayCastPrivate( _rootItem, ray, XTransform(), result, exact, X_UINT32_SENTINEL, XList<xuint32>() );
  return result;
  }

bool XEnvironmentRenderer::exactRayCast( ItemID id,
                                 const XLine &ray,
                                 const XTransform &worldTransform,
                                 RayCastResult::HitList &hits )
  {
  XLine localLine( worldTransform.inverted() * ray );
  bool result = false;

  XEnvironment::ReadLock lock(environment());
  const Area *area = environment()->area(id);

  foreach(const Area::ShadingGroup &group, area->shadingGroups())
    {
    foreach(const Area::MeshPair &mesh, group.meshes())
      {
      const  *area = environment()->mesh(mesh.mesh());
      XVector3DList posList;
      XList <unsigned int> tris;
      if( XMeshUtilities::intersect( "vertex", localLine, shape.geometry(), posList, tris ) )
        {
        for( int x=0; x<posList.size(); ++x )
          {
          hits << RayCastResult::Hit( &(shape.geometry()), posList[x], tris[x] );
          }
        result = true;
        }
      }
    }

  environment()->endItemLock(id);
  return result;
  }

void XEnvironmentRenderer::rayCastPrivate( ItemID id,
                                   const XLine &ray,
                                   const XTransform &worldTransform,
                                   RayCastResults &results,
                                   bool exact,
                                   xuint32 index,
                                   XList<xuint32> parentTree )
  {
  const Item *item = environment()->beginItemLockBlocking(id);

  RayCastResults subResults;
  if((worldTransform * item->bounds()).intersects(ray))
    {
    XList<xuint32> subParentTree = parentTree;
    subParentTree << id;

    // start at minus one because this loop can exit early,
    // so we increment it at the start.
    xuint32 childIndex = -1;
    foreach(const Child &ch, item->children())
      {
      childIndex++;
      XTransform childWorldTransform = ch.transform() * worldTransform;

      if( ch.hasDistanceCheck() )
        {
        float distance = (childWorldTransform.translation() - viewer()->position()).lengthSquared();

        if(distance < ch.minimumDistanceSquared() ||
           distance > ch.maximumDistanceSquared() )
          {
          continue;
          }
        }

      rayCastPrivate( ch.ID(), ray, childWorldTransform, subResults, exact, childIndex, subParentTree );
      }

    environment()->endItemLock(id);

    RayCastResult::HitList hits;
    if( subResults.size() || ( exact && exactRayCast( id, ray, worldTransform, hits ) ) )
      {
      results << RayCastResult( id, worldTransform, subResults, parentTree, index, hits );
      }
    }
  else
    {
    environment()->endItemLock(id);
    }
  }

void XEnvironmentRenderer::lightChanged(xuint16 light)
  {
  xAssert(light < _lightsNeedRefresh.size());
  _lightsNeedRefresh[light] = true;
  }

void XEnvironmentRenderer::numberOfLightsChanged(xuint16 light)
  {
  _lightsNeedRefresh.resize(light);
  }

#endif
