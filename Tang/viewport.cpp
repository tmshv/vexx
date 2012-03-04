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
#include "MCCube.h"
#include "object.h"

class FractalGeometry : public SEntity
  {
  S_PROPERTY(FractalGeometry, SEntity, 0)
public:
  class Region
    {
  XProperties:
    XRefProperty(xuint32, x);
    XRefProperty(xuint32, y);
    XRefProperty(xuint32, z);
    XRefProperty(xuint32, width);
    XRefProperty(xuint32, height);
    XRefProperty(xuint32, depth);
    XROProperty(FractalGeometry *, fractalGeometry);

  public:
    Region(FractalGeometry * f, xuint32 x, xuint32 y, xuint32 z)
        : _x(x), _y(y), _z(z), _width(f->width()), _height(f->height()), _depth(f->depth()), _fractalGeometry(f)
      {
      }

    enum PointCase
      {
      XYZ = 1,
      WYZ = 2,
      WWZ = 4,
      XWZ = 8,
      XYW = 16,
      WYW = 32,
      WWW = 64,
      XWW = 128
      };

    xuint32 getPointIndex(xuint8 c) const
      {
      xuint32 offset = 0;
      switch(c)
        {
      case XYZ: break;
      case WYZ: offset += 1; break;
      case WWZ: offset += 1 + _width; break;
      case XWZ: offset += _width; break;
      case XYW: offset += (_height * _width); break;
      case WYW: offset += 1 + (_height * _width); break;
      case WWW: offset += 1 + _width + (_height * _width); break;
      case XWW: offset += _width + (_height * _width); break;
      default: xAssertFail(); break;
        }
      return fractalGeometry()->indexAtPoint(x(), y(), z()) + offset;
      }
    };

  UnsignedIntProperty width;
  UnsignedIntProperty height;
  UnsignedIntProperty depth;

  SFloatArrayProperty densitySamples;

  GCGeometry geometry;

  XVector3D pointAtIndex(xsize x, xsize y, xsize z) const
    {
    return XVector3D(x, y, z);
    }

  XVector3D pointAtIndex(xsize pt) const
    {
    xuint32 w = width();
    xuint32 h = height();

    xuint32 z = pt / (w * h);
    xuint32 left = pt % (w * h);
    xuint32 y = left / w;
    xuint32 x = left % w;

    return pointAtIndex(x, y, z);
    }

  xsize indexAtPoint(xsize x, xsize y, xsize z) const
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
  class Utils
    {
  public:
    static void addPoint(const XVector3D &pointA,
                                   const XVector3D &pointB,
                                   float densityA,
                                   float densityB,
                                   XVector<XVector3D>& vertex)
      {
      xAssert((densityA > 0.0f && densityB < 0.0f) || (densityA < 0.0f && densityB > 0.0f));
      float percentAlong = fabs(densityA) / (fabs(densityA) + fabs(densityB));

      XVector3D point = pointA + (percentAlong * pointB);
      vertex << point;
      }

    static void addIndexRing(XVector<xuint32> &vertexIndices, xuint32 initial, xuint32 *offsets, xsize numOffsets, bool reverse)
      {
      for(xsize i=0; i<numOffsets; ++i)
        {
        xuint32 idx = reverse ? offsets[numOffsets - 1 - i] : offsets[i];
        idx += initial;

        vertexIndices << idx;
        }
      }

    enum Axis
      {
      XAxis,
      YAxis,
      ZAxis
      };

    static xuint8 flipPointInAxis(xuint8 c, Axis ax)
      {
      if(ax == XAxis)
        {
        switch(c)
          {
        case FractalGeometry::Region::XYZ: return FractalGeometry::Region::WYZ;
        case FractalGeometry::Region::WYZ: return FractalGeometry::Region::XYZ;
        case FractalGeometry::Region::WWZ: return FractalGeometry::Region::XWZ;
        case FractalGeometry::Region::XWZ: return FractalGeometry::Region::WWZ;
        case FractalGeometry::Region::XYW: return FractalGeometry::Region::WYW;
        case FractalGeometry::Region::WYW: return FractalGeometry::Region::XYW;
        case FractalGeometry::Region::WWW: return FractalGeometry::Region::XWW;
        case FractalGeometry::Region::XWW: return FractalGeometry::Region::WWW;
        default: xAssertFail(); break;
          }
        }
      else if(ax == YAxis)
        {
        switch(c)
          {
        case FractalGeometry::Region::XYZ: return FractalGeometry::Region::XWZ;
        case FractalGeometry::Region::WYZ: return FractalGeometry::Region::WWZ;
        case FractalGeometry::Region::WWZ: return FractalGeometry::Region::WYZ;
        case FractalGeometry::Region::XWZ: return FractalGeometry::Region::XYZ;
        case FractalGeometry::Region::XYW: return FractalGeometry::Region::XWW;
        case FractalGeometry::Region::WYW: return FractalGeometry::Region::WWW;
        case FractalGeometry::Region::WWW: return FractalGeometry::Region::WYW;
        case FractalGeometry::Region::XWW: return FractalGeometry::Region::XYW;
        default: xAssertFail(); break;
          }
        }
      else if(ax == ZAxis)
        {
        switch(c)
          {
        case FractalGeometry::Region::XYZ: return FractalGeometry::Region::XYW;
        case FractalGeometry::Region::WYZ: return FractalGeometry::Region::WYW;
        case FractalGeometry::Region::WWZ: return FractalGeometry::Region::WWW;
        case FractalGeometry::Region::XWZ: return FractalGeometry::Region::XWW;
        case FractalGeometry::Region::XYW: return FractalGeometry::Region::XYZ;
        case FractalGeometry::Region::WYW: return FractalGeometry::Region::WYZ;
        case FractalGeometry::Region::WWW: return FractalGeometry::Region::WWZ;
        case FractalGeometry::Region::XWW: return FractalGeometry::Region::WYZ;
        default: xAssertFail(); break;
          }
        }
      xAssertFail();
      return 0;
      }

    static xuint8 pointFromCase(xuint8 c, xuint8 num)
      {
      for(xuint32 i=0; i<8; ++i)
        {
        if((c&1) == 1)
          {
          if(num == 0)
            {
            return 1 << i;
            }
          else
            {
            --num;
            }
          }

        c = c >> 1;
        }

      xAssertFail();
      return 1;
      }

    static void addVerticesAndIndicesForCase(const FractalGeometry *fr, const FractalGeometry::Region &region, xuint8 boxCase, XVector<XVector3D> &vertices, const float *densities, XVector<xuint32> &vertexIndices)
      {

#define ADD_INDEX_RING(...) \
      { xuint32 data[] = { __VA_ARGS__ }; \
      addIndexRing(vertexIndices, base, data, sizeof(data)/sizeof(xuint32), reverse); };

#define GET_POINT(point) fr->pointAtIndex(region.getPointIndex(point))
#define GET_POINT_FLIPPED(point, axis) GET_POINT(Utils::flipPointInAxis(point, axis))

#define GET_DENSITY(point) densities[region.getPointIndex(point)]
#define GET_DENSITY_FLIPPED(point, axis) GET_DENSITY(Utils::flipPointInAxis(point, axis))

#define ADD_POINT(point, axis) { XVector3D a = GET_POINT(point), b = GET_POINT_FLIPPED(point, axis); \
      float dA = GET_DENSITY(point), dB = GET_DENSITY_FLIPPED(point, axis); \
      addPoint(a, b, dA, dB, vertices); }

//       8 __________ 7
//        /|         /|
//       / |        / |
//    4 /__|_____3_/  |
//      |  |       |  | 6
//   /\ |  |5______|  / /|
//    Y | /        | / Z
//      |__________|/
//     1   X >      2

      bool reverse = (((boxCase % 2) == 1) && (boxCase != 0)) || (boxCase == 254);
      XFlags<xuint8, xuint8> normalisedCase = reverse ? ~boxCase : boxCase;

#define TEST_CASE(c, var) (var = c, normalisedCase.hasFlag(c))
      xuint8 chosenCase = 0;
      while(normalisedCase != 0)
        {
        xuint32 base = vertices.size();

        // X Half missing
        if(TEST_CASE(FractalGeometry::Region::WYZ|FractalGeometry::Region::WWZ|FractalGeometry::Region::WWW|FractalGeometry::Region::WYW, chosenCase))
          {
          xuint8 pointA = pointFromCase(chosenCase, 0);
          xuint8 pointB = pointFromCase(chosenCase, 1);
          xuint8 pointC = pointFromCase(chosenCase, 2);
          xuint8 pointD = pointFromCase(chosenCase, 3);
          ADD_POINT(pointA, XAxis);
          ADD_POINT(pointB, XAxis);
          ADD_POINT(pointC, XAxis);
          ADD_POINT(pointD, XAxis);
          ADD_INDEX_RING(0,1,2,3);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }

        // Y Half missing
        if(TEST_CASE(FractalGeometry::Region::XYZ|FractalGeometry::Region::WYZ|FractalGeometry::Region::WYW|FractalGeometry::Region::XYW, chosenCase))
          {
          xuint8 pointA = pointFromCase(chosenCase, 0);
          xuint8 pointB = pointFromCase(chosenCase, 1);
          xuint8 pointC = pointFromCase(chosenCase, 2);
          xuint8 pointD = pointFromCase(chosenCase, 3);
          ADD_POINT(pointA, YAxis);
          ADD_POINT(pointB, YAxis);
          ADD_POINT(pointC, YAxis);
          ADD_POINT(pointD, YAxis);
          ADD_INDEX_RING(0,1,2,3);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }

        // Z Half missing
        if(TEST_CASE(FractalGeometry::Region::XYZ|FractalGeometry::Region::WYZ|FractalGeometry::Region::WWZ|FractalGeometry::Region::XWZ, chosenCase))
          {
          xuint8 pointA = pointFromCase(chosenCase, 0);
          xuint8 pointB = pointFromCase(chosenCase, 1);
          xuint8 pointC = pointFromCase(chosenCase, 2);
          xuint8 pointD = pointFromCase(chosenCase, 3);
          ADD_POINT(pointA, ZAxis);
          ADD_POINT(pointB, ZAxis);
          ADD_POINT(pointC, ZAxis);
          ADD_POINT(pointD, ZAxis);
          ADD_INDEX_RING(0,1,2,3);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }

        // X three point-half missing
        if(TEST_CASE(FractalGeometry::Region::WYZ|FractalGeometry::Region::WWZ|FractalGeometry::Region::WWW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WYZ|FractalGeometry::Region::XWW|FractalGeometry::Region::WWW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WYZ|FractalGeometry::Region::WYW|FractalGeometry::Region::WWW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WYZ|FractalGeometry::Region::WWZ|FractalGeometry::Region::WYW, chosenCase))
          {
      xAssertFail();
          xuint8 pointA = pointFromCase(chosenCase, 0);
          xuint8 pointB = pointFromCase(chosenCase, 1);
          ADD_POINT(pointA, ZAxis);
          ADD_POINT(pointA, YAxis);
          ADD_POINT(pointB, YAxis);
          ADD_POINT(pointB, ZAxis);
          ADD_INDEX_RING(0,1,2,3);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }

        // X direction edge missing
        if(TEST_CASE(FractalGeometry::Region::XYZ|FractalGeometry::Region::WYZ, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::XYW|FractalGeometry::Region::WYW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::XWZ|FractalGeometry::Region::WWZ, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::XWW|FractalGeometry::Region::WWW, chosenCase))
          {
          xuint8 pointA = pointFromCase(chosenCase, 0);
          xuint8 pointB = pointFromCase(chosenCase, 1);
          ADD_POINT(pointA, ZAxis);
          ADD_POINT(pointA, YAxis);
          ADD_POINT(pointB, YAxis);
          ADD_POINT(pointB, ZAxis);
          ADD_INDEX_RING(0,1,2,3);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }

        // Y direction edge missing
        if(TEST_CASE(FractalGeometry::Region::XYZ|FractalGeometry::Region::XWZ, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::XYW|FractalGeometry::Region::XWW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WYW|FractalGeometry::Region::WWW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WYZ|FractalGeometry::Region::WWZ, chosenCase))
          {
          xuint8 pointA = pointFromCase(chosenCase, 0);
          xuint8 pointB = pointFromCase(chosenCase, 1);
          ADD_POINT(pointA, XAxis);
          ADD_POINT(pointA, ZAxis);
          ADD_POINT(pointB, ZAxis);
          ADD_POINT(pointB, XAxis);
          ADD_INDEX_RING(0,1,2,3);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }

        // Z direction edge missing
        if(TEST_CASE(FractalGeometry::Region::XYZ|FractalGeometry::Region::XYW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WYZ|FractalGeometry::Region::WYW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WWZ|FractalGeometry::Region::WWW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::XWZ|FractalGeometry::Region::XWW, chosenCase))
          {
          xuint8 pointA = pointFromCase(chosenCase, 0);
          xuint8 pointB = pointFromCase(chosenCase, 1);
          ADD_POINT(pointA, XAxis);
          ADD_POINT(pointA, YAxis);
          ADD_POINT(pointB, YAxis);
          ADD_POINT(pointB, XAxis);
          ADD_INDEX_RING(0,1,2,3);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }

        // one point
        if(TEST_CASE(FractalGeometry::Region::XYZ, chosenCase) || TEST_CASE(FractalGeometry::Region::WYZ, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WWZ, chosenCase) || TEST_CASE(FractalGeometry::Region::XWZ, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::XYW, chosenCase) || TEST_CASE(FractalGeometry::Region::WYW, chosenCase) ||
           TEST_CASE(FractalGeometry::Region::WWW, chosenCase) || TEST_CASE(FractalGeometry::Region::XWW, chosenCase))
          {
          xuint8 point = pointFromCase(chosenCase, 0);
          ADD_POINT(point, XAxis);
          ADD_POINT(point, YAxis);
          ADD_POINT(point, ZAxis);
          ADD_INDEX_RING(0,1,2);

          normalisedCase.clearFlag(chosenCase);
          continue;
          }
        }
      }
    };

  FractalGeometry *fr = cont->uncheckedCastTo<FractalGeometry>();

  xsize width = fr->width();
  xsize height = fr->height();
  xsize depth = fr->depth();

  const SFloatArrayProperty::EigenArray densities = fr->densitySamples.data();

  XVector<XVector3D> vertices;
  vertices.reserve(256);

  XVector<xuint32> vertexIndices;
  vertexIndices.reserve(256);

  for(xsize z=0; z<(depth-1); ++z)
    {
    for(xsize y=0; y<(height-1); ++y)
      {
      for(xsize x=0; x<(width-1); ++x)
        {
        FractalGeometry::Region region(fr, x, y, z);

        xsize indexXYZ = region.getPointIndex(FractalGeometry::Region::XYZ);
        xsize indexWYZ = region.getPointIndex(FractalGeometry::Region::WYZ);
        xsize indexWWZ = region.getPointIndex(FractalGeometry::Region::WWZ);
        xsize indexXWZ = region.getPointIndex(FractalGeometry::Region::XWZ);
        xsize indexXYW = region.getPointIndex(FractalGeometry::Region::XYW);
        xsize indexWYW = region.getPointIndex(FractalGeometry::Region::WYW);
        xsize indexWWW = region.getPointIndex(FractalGeometry::Region::WWW);
        xsize indexXWW = region.getPointIndex(FractalGeometry::Region::XWW);

        xuint8 boxCase = 0;
        boxCase |= ((densities(indexXYZ) > 0.0f) ? FractalGeometry::Region::XYZ : 0);
        boxCase |= ((densities(indexWYZ) > 0.0f) ? FractalGeometry::Region::WYZ : 0);
        boxCase |= ((densities(indexWWZ) > 0.0f) ? FractalGeometry::Region::WWZ : 0);
        boxCase |= ((densities(indexXWZ) > 0.0f) ? FractalGeometry::Region::XWZ : 0);
        boxCase |= ((densities(indexXYW) > 0.0f) ? FractalGeometry::Region::XYW : 0);
        boxCase |= ((densities(indexWYW) > 0.0f) ? FractalGeometry::Region::WYW : 0);
        boxCase |= ((densities(indexWWW) > 0.0f) ? FractalGeometry::Region::WWW : 0);
        boxCase |= ((densities(indexXWW) > 0.0f) ? FractalGeometry::Region::XWW : 0);

        if(boxCase == 0 || boxCase == 255)
          {
          continue;
          }


        Utils::addVerticesAndIndicesForCase(fr, region, boxCase, vertices, densities.data(), vertexIndices);
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

  MCCube *cube = msc->addChild<MCCube>("Cube");
  transform->geometry.setPointed(&cube->geometry);

  STypeRegistry::addType(FractalGeometry::staticTypeInformation());
  FractalGeometry *geo = msc->addChild<FractalGeometry>("FractalGeometry");
  transform2->geometry.setPointed(&geo->geometry);

  vp->source.setPointed(msc);
  }

Viewport::~Viewport()
  {
  }

void Viewport::setObject(Object *)
  {
  }
