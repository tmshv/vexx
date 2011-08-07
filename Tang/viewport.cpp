#include "viewport.h"
#include "QApplication"
#include "QTimer"
#include "QAction"
#include "QKeyEvent"
#include "QFile"
#include "QMouseEvent"
#include "QVBoxLayout"
#include "XGLRenderer.h"
#include "XScene.h"
#include "UI/sentityui.h"
#include "3D/GCCamera.h"
#include "3D/GCViewport.h"
#include "3D/GCScene.h"
#include "3D/Shaders/GCStandardSurface.h"
#include "3D/GCShadingGroup.h"
#include "3D/GCScreenRenderTarget.h"
#include "3D/Renderable/GCCuboid.h"
#include "3D/GCTexture.h"
#include "3D/Renderable/GCPlane.h"
#include "object.h"

class FractalGeometry : public SEntity
  {
  S_PROPERTY(FractalGeometry, SEntity, 0)
public:

  UnsignedIntProperty width;
  UnsignedIntProperty height;
  UnsignedIntProperty depth;

  SFloatArrayProperty densitySamples;

  GCGeometry geometry;

  XVector3D pointAtIndex(xsize x, xsize y, xsize z)
    {
    return XVector3D(x, y, z);
    }

  xsize indexAtPoint(xsize x, xsize y, xsize z)
    {
    return x + (y * width()) + (z * height() * width());
    }

  float densityAtPoint(const XVector3D &point)
    {
    XVector3D mid(width(), height(), depth());
    mid *= 0.5;

    XVector3D diff = point - diff;

    return diff.norm() - mid.x();
    }
  };

S_IMPLEMENT_PROPERTY(FractalGeometry)

void computeSamples(const SPropertyInstanceInformation *, SPropertyContainer *cont)
  {
  FractalGeometry *fr = cont->uncheckedCastTo<FractalGeometry>();

  xsize size = fr->width() * fr->height() * fr->depth();
  SFloatArrayProperty::EigenArray arr(size, 1);

  for(xsize z=0, zS = fr->depth(); z<zS; ++z)
    {
    for(xsize y=0, yS = fr->height(); y<yS; ++y)
      {
      for(xsize x=0, xS = fr->width(); x<xS; ++x)
        {
        xsize index = fr->indexAtPoint(x, y, z);
        arr(index) = fr->densityAtPoint(fr->pointAtIndex(x, y, z));
        }
      }
    }

  fr->densitySamples.setData(arr);
  }

void computeGeometry(const SPropertyInstanceInformation *, SPropertyContainer *cont)
  {
  enum EdgeAxis
    {
    XEdge = 1,
    YEdge = 2,
    ZEdge = 3
    };

  enum OffsetAxis
    {
    NegNeg = 0,
    NegPos = 4,
    PosNeg = 8,
    PosPos = 12
    };

  enum Masks
    {
    EdgeMask = 0x3,
    OffsetMask = 0xC
    };

  enum EdgeIdentifier
    {
    NoIdentifier = 0,

    XNegNeg = 1,
    XNegPos = 2,
    XPosNeg = 4,
    XPosPos = 8,

    YNegNeg = 16,
    YNegPos = 32,
    YPosNeg = 64,
    YPosPos = 128,

    ZNegNeg = 256,
    ZNegPos = 512,
    ZPosNeg = 1024,
    ZPosPos = 2048
    };

  class Utils
    {
  public:
    static bool addPointIfRequired(FractalGeometry *fr,
                                   const XVector3D &pointA,
                                   const XVector3D &pointB,
                                   float densityA,
                                   float densityB,
                                   XVector<XVector3D>& vertex,
                                   xsize &index)
      {
      if((densityA > 0.0f && densityB < 0.0f) || (densityA < 0.0f && densityB > 0.0f))
        {
        float percentAlong = fabs(densityA) / (fabs(densityA) + fabs(densityB));

        XVector3D point = pointA + (percentAlong * pointB);

        index = vertex.size();
        vertex << point;

        return true;
        }

      return false;
      }

    static xsize indexInBox(xsize x, xsize y, xsize z, xsize width, xsize height, xsize)
      {
      return x + (y * (width-1)) + (z * (height-1) * (width-1));
      }

    static xsize toEdgeIdentifier(int id)
      {
      switch(id&EdgeMask)
        {
        case XEdge:
          {
          switch(id&OffsetMask)
            {
            case NegNeg:
              return XNegNeg;
            case PosNeg:
              return XPosNeg;
            case NegPos:
              return XNegPos;
            case PosPos:
              return XPosPos;
            }
          }
          break;
        case YEdge:
          {
          switch(id&OffsetMask)
            {
            case NegNeg:
              return YNegNeg;
            case PosNeg:
              return YPosNeg;
            case NegPos:
              return YNegPos;
            case PosPos:
              return YPosPos;
            }
          }
          break;
        case ZEdge:
          {
          switch(id&OffsetMask)
            {
            case NegNeg:
              return ZNegNeg;
            case PosNeg:
              return ZPosNeg;
            case NegPos:
              return ZNegPos;
            case PosPos:
              return ZPosPos;
            }
          }
          break;
        }
      xAssertFail();
      return NoIdentifier;
      }

    static void markIndex(EdgeAxis axis, xsize *indices, xsize x, xsize y, xsize z, xsize width, xsize height, xsize depth)
      {
      // mark axis
      xsize index = indexInBox(x, y, z, width, height, depth);
      indices[index] |= toEdgeIdentifier(axis|NegNeg);

      // consider upper axes
      if(axis != XEdge && (x+1) < width)
        {
        int type = (axis == YEdge) ? NegPos : PosNeg;
        xsize index = indexInBox(x+1, y, z, width, height, depth);
        indices[index] |= toEdgeIdentifier(axis|type);
        }
      if(axis != YEdge && (y+1) < height)
        {
        int type = (axis == XEdge) ? PosNeg : NegPos;
        xsize index = indexInBox(x, y+1, z, width, height, depth);
        indices[index] |= toEdgeIdentifier(axis|type);
        }
      if(axis != ZEdge && (z+1) < depth)
        {
        int type = axis == XEdge ? NegPos : PosNeg;
        xsize index = indexInBox(x, y, z+1, width, height, depth);
        indices[index] |= toEdgeIdentifier(axis|type);
        }

      if(axis == XEdge && (y+1) < height && (z+1) < depth)
        {
        xsize index = indexInBox(x, y+1, z+1, width, height, depth);
        indices[index] |= toEdgeIdentifier(axis|PosPos);
        }
      if(axis == YEdge && (x+1) < width && (z+1) < depth)
        {
        xsize index = indexInBox(x+1, y, z+1, width, height, depth);
        indices[index] |= toEdgeIdentifier(axis|PosPos);
        }
      if(axis == ZEdge && (x+1) < width && (y+1) < height)
        {
        xsize index = indexInBox(x+1, y+1, z, width, height, depth);
        indices[index] |= toEdgeIdentifier(axis|PosPos);
        }
      }

    static void addVertexIndices(xsize boxType)
      {
      }
    };

  FractalGeometry *fr = cont->uncheckedCastTo<FractalGeometry>();

  xsize width = fr->width();
  xsize height = fr->height();
  xsize depth = fr->depth();

  STemporaryMemory <xsize> indicesAlloc(fr, (width-1)*(height-1)*(depth-1));
  xsize *indices = indicesAlloc.data();

  STemporaryMemory <XVector3D> pointsAlloc(fr, width*height*depth);
  XVector3D *points = pointsAlloc.data();

  const SFloatArrayProperty::EigenArray densities = fr->densitySamples.data();

  XVector<XVector3D> vertices;
  vertices.reserve(256);


  for(xsize z=0; z<depth; ++z)
    {
    for(xsize y=0; y<height; ++y)
      {
      for(xsize x=0; x<width; ++x)
        {
        xsize index = fr->indexAtPoint(x, y, z);
        points[index] = fr->pointAtIndex(x, y, z);
        }
      }
    }

  for(xsize z=0; z<depth; ++z)
    {
    for(xsize y=0; y<height; ++y)
      {
      for(xsize x=0; x<width; ++x)
        {
        xsize indexXYZ = fr->indexAtPoint(x, y, z);
        XVector3D pointXYZ = points[indexXYZ];
        float densityXYZ = densities(indexXYZ);

        if((x+1) < width)
          {
          xsize indexWYZ = fr->indexAtPoint(x+1, y, z);
          XVector3D pointWYZ = points[indexWYZ];
          float densityWYZ = densities(indexWYZ);

          xsize index = X_SIZE_SENTINEL;
          if(Utils::addPointIfRequired(fr, pointXYZ, pointWYZ, densityXYZ, densityWYZ, vertices, index))
            {
            Utils::markIndex(XEdge, indices, x, y, z, width, depth, height);
            }
          }

        if((y+1) < height)
          {
          xsize indexXWZ = fr->indexAtPoint(x, y+1, z);
          XVector3D pointXWZ = points[indexXWZ];
          float densityXWZ = densities(indexXWZ);

          xsize index = X_SIZE_SENTINEL;
          if(Utils::addPointIfRequired(fr, pointXYZ, pointXWZ, densityXYZ, densityXWZ, vertices, index))
            {
            Utils::markIndex(YEdge, indices, x, y, z, width, depth, height);
            }
          }

        if((z+1) < depth)
          {
          xsize indexXYW = fr->indexAtPoint(x, y, z+1);
          XVector3D pointXYW = points[indexXYW];
          float densityXYW = densities(indexXYW);

          xsize index = X_SIZE_SENTINEL;
          if(Utils::addPointIfRequired(fr, pointXYZ, pointXYZ, densityXYZ, densityXYW, vertices, index))
            {
            Utils::markIndex(YEdge, indices, x, y, z, width, depth, height);
            }
          }
        }
      }
    }

  XVector<xuint32> vertexIndices;
  vertexIndices.reserve(256);

  for(xsize z=0; z<(depth-1); ++z)
    {
    for(xsize y=0; y<(height-1); ++y)
      {
      for(xsize x=0; x<(width-1); ++x)
        {
        xsize index = indexInBox(x, y, z, width, height, depth);
        Utils::addVertexIndices(indices[index]);
        }
      }
    }

  //fr->geometry;
  }

SPropertyInformation *FractalGeometry::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<FractalGeometry>("FractalGeometry");

  GCGeometry::InstanceInformation *geoInfo = info->add(&FractalGeometry::geometry, "geometry");
  geoInfo->setCompute(computeGeometry);

  SFloatArrayProperty::InstanceInformation *samplesInfo = info->add(&FractalGeometry::densitySamples, "densitySamples");
  samplesInfo->setAffects(geoInfo);
  samplesInfo->setCompute(computeSamples);

  UnsignedIntProperty::InstanceInformation *iInfo = info->add(&FractalGeometry::width, "width");
  iInfo->setAffects(samplesInfo);
  iInfo->setDefault(5);

  iInfo = info->add(&FractalGeometry::height, "height");
  iInfo->setAffects(samplesInfo);
  iInfo->setDefault(5);

  iInfo = info->add(&FractalGeometry::depth, "depth");
  iInfo->setAffects(samplesInfo);
  iInfo->setDefault(5);

  return info;
  }
  
Viewport::Viewport(SPlugin &db) : SViewport(db.db().addChild<GCViewport>("SomeScene")), UISurface("Viewport", this, UISurface::Dock)
  {
  GCViewport *vp = viewport();

  GCPerspectiveCamera* cam = vp->addChild<GCPerspectiveCamera>("Camera");
  vp->x.connect(&cam->viewportX);
  vp->y.connect(&cam->viewportY);
  vp->width.connect(&cam->viewportWidth);
  vp->height.connect(&cam->viewportHeight);

  cam->setPosition(XVector3D(0.0f, 0.0f, 10.0f));
  cam->setFocalPoint(XVector3D(0.0f, 0.0f, 0.0f));

  GCManipulatableScene* msc = vp->addChild<GCManipulatableScene>("Scene");
  cam->projection.connect(&msc->cameraProjection);
  cam->viewTransform.connect(&msc->cameraTransform);
  cam->connect(&msc->activeCamera);
  setController(msc);

  GCShadingGroup *group = msc->addChild<GCShadingGroup>("Groups");
  GCShadingGroup *group2 = msc->addChild<GCShadingGroup>("Groups");
  msc->shadingGroups.addPointer(group);
  msc->shadingGroups.addPointer(group2);

  const SPropertyInformation *standardSurfaceInfo = STypeRegistry::findType("StandardSurface");
  SProperty *shader = msc->addChild(standardSurfaceInfo, "Shader");
  group->shader.setPointed(shader->uncheckedCastTo<GCShader>());

  const SPropertyInformation *flatSurfaceInfo = STypeRegistry::findType("FlatSurface");
  SProperty *shader2 = msc->addChild(flatSurfaceInfo, "Shader");
  GCShader *shaderS = shader2->uncheckedCastTo<GCShader>();
  group2->shader.setPointed(shaderS);

  GCTexture *t = msc->addChild<GCTexture>("Texture");
  shaderS->findChild("texture")->uncheckedCastTo<GCTexturePointer>()->setPointed(t);


  XTransform tr = XTransform::Identity();
  tr.translation() = XVector3D(1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform = msc->addChild<GCGeometryTransform>("Transform");
  group->geometry.addPointer(transform);
  transform->transform = tr;


  tr.translation() = XVector3D(-1.0f, 0.0f, 0.0f);

  GCGeometryTransform *transform2 = msc->addChild<GCGeometryTransform>("Transform");
  group2->geometry.addPointer(transform2);
  transform2->transform = tr;

  GCCuboid *cube = msc->addChild<GCCuboid>("Cube");
  transform->geometry.setPointed(&cube->geometry);

  FractalGeometry *geo = _db->addChild<FractalGeometry>("FractalGeometry");
  transform2->geometry.setPointed(&geo->geometry);

  op->source.setPointed(scene);
  }

Viewport::~Viewport()
  {
  }

void Viewport::setObject(Object *)
  {
  }
