#include "GCGeometry.h"
#include "styperegistry.h"
#include "sprocessmanager.h"

S_IMPLEMENT_PROPERTY(GCGeometryAttribute)

SPropertyInformation *GCGeometryAttribute::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometryAttribute>("GCGeometryAttribute");
  info->add(&GCGeometryAttribute::polygons, "polygons");
  return info;
  }

GCGeometryAttribute::GCGeometryAttribute()
  {
  }

void GCGeometryAttribute::setType(const SPropertyInformation *type)
  {
  SBlock b(database());
  if(size() > 1)
    {
    removeProperty(firstChild());
    }

  addProperty(type);
  }

void GCGeometryAttribute::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  xuint32 expandBy = 0;
  for(xuint32 i=0; i<count; ++i)
    {
    expandBy += 1 + sizes[i];
    }

  SUIntArrayProperty::EigenArray data = polygons.data();
  xuint32 offset = data.size();

  data.resize(offset + expandBy, 1);

  for(xuint32 i=0; i<count; ++i)
    {
    data(offset) = sizes[i];
    offset += 1;

    // initialise indices to 0.
    for(xuint32 j=0; j<sizes[i]; ++j)
      {
      data(offset + j) = 0;
      }

    offset += sizes[i];
    }

  polygons.setData(data);
  }

void GCGeometryAttribute::removePolygons(xuint32 index, xuint32 count)
  {
  SUIntArrayProperty::EigenArray data = polygons.data();

  xuint32 currentIndex = 0;

  xuint32 startOffset = X_UINT32_SENTINEL;

  xuint32 initialSize = data.size();
  xuint32 offset = 0;
  while(offset < initialSize)
    {
    if(index == currentIndex && startOffset == X_UINT32_SENTINEL)
      {
      startOffset = offset;
      }
    else if(index == currentIndex+count)
      {
      xAssert(startOffset != X_UINT32_SENTINEL);

      // offset of the last index we are removing
      xuint32 finalOffset = offset + data(offset);

      // assign the important data over the old data.
      data.block(startOffset, 0, initialSize - finalOffset, 1) = data.block(finalOffset, 0, initialSize - finalOffset, 1);

      // remove the end of the array we dont want.
      data.resize(initialSize - (finalOffset - startOffset), 1);

      polygons.setData(data);
      return;
      }

    // increment by the poly size and the indices.
    offset += 1 + data(offset);
    currentIndex += 1;
    }

  xAssertFail();
  }

void GCGeometryAttribute::setPolygon(xuint32 index, const xuint32 *indices)
  {
  SUIntArrayProperty::EigenArray data = polygons.data();

  xuint32 currentIndex = 0;

  xuint32 initialSize = data.size();
  xuint32 offset = 0;
  while(offset < initialSize)
    {
    if(index == currentIndex)
      {
      xuint32* rawData = data.data();
      rawData += offset + 1;

      memcpy(rawData, indices, sizeof(xuint32) * data(offset));

      polygons.setData(data);
      return;
      }

    // increment by the poly size and the indices.
    offset += 1 + data(offset);
    currentIndex += 1;
    }

  xAssertFail();
  }

S_IMPLEMENT_PROPERTY(GCGeometry)

void computeRuntimeGeometry(const SPropertyInstanceInformation *, SPropertyContainer *cont)
  {
  xAssert(SProcessManager::isMainThread());

  GCGeometry* rtGeo = cont->uncheckedCastTo<GCGeometry>();

  XGeometry x;
  rtGeo->appendTo(&x);
  rtGeo->runtimeGeometry = x;
  }

SPropertyInformation *GCGeometry::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCGeometry>("GCGeometry");

  GCRuntimeGeometry::InstanceInformation *rtGeo = info->add(&GCGeometry::runtimeGeometry, "runtimeGeometry");
  rtGeo->setCompute(computeRuntimeGeometry);
  rtGeo->setComputeLockedToMainThread(true);

  STypedPropertyArray<GCGeometryAttribute>::InstanceInformation *attrs = info->add(&GCGeometry::attributes, "attributes");
  attrs->setAffects(rtGeo);

  return info;
  }

GCGeometry::GCGeometry()
  {
  }

void GCGeometry::addPolygons(const xuint32 *sizes, xuint32 count)
  {
  for(GCGeometryAttribute *child=attributes.firstChild<GCGeometryAttribute>(); child; child=child->nextSibling<GCGeometryAttribute>())
    {
    child->addPolygons(sizes, count);
    }
  }

void GCGeometry::removePolygons(xuint32 index, xuint32 count)
  {
  for(GCGeometryAttribute *child=attributes.firstChild<GCGeometryAttribute>(); child; child=child->nextSibling<GCGeometryAttribute>())
    {
    child->removePolygons(index, count);
    }
  }

namespace Triangulator
  {
  void triangulate(const xuint32 *,
                          const XVector3D *,
                          xuint32 polySize,
                          XVector<xuint32>* triangleData,
                          xuint32 vertexStart)
    {
    xuint32 zeroVertex = vertexStart++;
    while(polySize > 2)
      {
      *triangleData << zeroVertex;

      --polySize;

      *triangleData << vertexStart;
      *triangleData << ++vertexStart;
      }
    }
  };

template <typename T> void appendToAttribute(XGeometry *geo, const QHash<QString, QVector<typename T::ElementType>> &attrDatas, T *attr, const QString &name, xuint32 size, const xuint32 *indices)
  {
  QVector<typename T::ElementType> attrData = attrDatas[name];
  attrData.reserve(attrData.size() + size);

  const typename T::EigenArray &arr = attr->data();
  const typename T::ElementType* data = arr.data();
  xAssert(data);

  for(xuint32 i=0; i<size; ++i)
    {
    xuint32 index = indices[i];
    xAssert(index < (xuint32)arr.rows());
    attrData << data[index];
    }

  geo->setAttribute(name, attrData);
  }

void GCGeometry::appendTo(XGeometry *geo) const
  {
  XVector <xuint32> triangles = geo->triangles();
  xuint32 vertexIndexStart = geo->attributes3D()["vertex"].size();

  const GCGeometryAttribute *positionAttr = attribute("vertex");
  if(!positionAttr)
    {
    return;
    }

  const SVector3ArrayProperty *vector3Data = positionAttr->attributeData<SVector3ArrayProperty>();
  if(!vector3Data)
    {
    return;
    }

  const SVector3ArrayProperty::EigenArray &positionArray = vector3Data->data();
  if(positionArray.rows() == 0)
    {
    return;
    }

  const XVector3D* positionData = positionArray.data();

  const SUIntArrayProperty::EigenArray &indices = positionAttr->polygons.data();
  xuint32 offset = 0;
  while(offset < (xuint32)indices.rows())
    {
    xuint32 indexCount = indices(offset);
    const xuint32 *polyIndex = indices.data() + offset + 1;

    // stream to vertex attributes at polyIndex from polygon, to index vertexIndexStart to (vertexIndexStart + indexCount)
    for(GCGeometryAttribute *child=attributes.firstChild<GCGeometryAttribute>(); child; child=child->nextSibling<GCGeometryAttribute>())
      {
      const SUIntArrayProperty::EigenArray &attrIndicesArray = child->polygons.data();
      xAssert(indexCount == attrIndicesArray(offset));
      const xuint32 *attrIndices = attrIndicesArray.data() + offset + 1;

      const SFloatArrayProperty* floatArray = child->attributeData<SFloatArrayProperty>();
      if(floatArray)
        {
        appendToAttribute(geo, geo->attributes1D(), floatArray, child->name(), indexCount, attrIndices);
        }
      else
        {
        const SVector2ArrayProperty* vec2 = child->attributeData<SVector2ArrayProperty>();
        if(vec2)
          {
          appendToAttribute(geo, geo->attributes2D(), vec2, child->name(), indexCount, attrIndices);
          }
        else
          {
          const SVector3ArrayProperty* vec3 = child->attributeData<SVector3ArrayProperty>();
          if(vec3)
            {
            appendToAttribute(geo, geo->attributes3D(), vec3, child->name(), indexCount, attrIndices);
            }
          else
            {
            const SVector4ArrayProperty* vec4 = child->attributeData<SVector4ArrayProperty>();
            if(vec4)
              {
              appendToAttribute(geo, geo->attributes4D(), vec4, child->name(), indexCount, attrIndices);
              }
            else
              {
              xAssertFailMessage("Non-supported attribute type encountered");
              }
            }
          }
        }
      }

    triangles.reserve(triangles.size() + (3 * (indexCount - 2)));
    Triangulator::triangulate(polyIndex, positionData, indexCount, &triangles, vertexIndexStart);

    vertexIndexStart += indexCount;
    offset += indexCount + 1;
    }

  geo->setTriangles(triangles);
  }

void GCGeometry::clearAttributes()
  {
  SBlock b(database());
  while(attributes.firstChild())
    {
    attributes.remove(attributes.firstChild());
    }
  }

