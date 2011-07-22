#ifndef GCABSTRACTNODEDELEGATE_H
#define GCABSTRACTNODEDELEGATE_H

#include "GCGlobal.h"
#include "XAbstractDelegate.h"
#include "QSize"
#include "QRect"
#include "QStaticText"
#include "QFontMetrics"
#include "QHash"

class QPoint;
class SEntity;

class GRAPHICSCORE_EXPORT GCAbstractNodeDelegate : public XAbstractDelegate
  {
public:
  enum HitArea
    {
    None,
    NodeDull,
    NodeTranslatable,
    Input,
    NodeOutput,
    Output,
    MaxNodeParts
    };

  // should be implemented to determine which area is hit on the node.
  virtual HitArea hitTest(const QPoint &point, const void *ent, xsize &index) const = 0;
  virtual void move(const QPoint &delta, const void *ent) const = 0;
  virtual void drawConnection(XAbstractCanvas *c, const void *ent, xsize prop, bool fromOutput, const QPoint &to) const = 0;
  };

class GRAPHICSCORE_EXPORT GCSimpleNodeDelegate : public GCAbstractNodeDelegate
  {
public:
  enum
    {
    ShadowPass,
    ConnectionPass,
    NodePass,
    MaxPasses
    };

  GCSimpleNodeDelegate();

  virtual void update(const XAbstractCanvas *, const XAbstractRenderModel::Iterator *, const XAbstractRenderModel *) const;
  virtual void paint(xuint32 pass, XAbstractCanvas *, const XAbstractRenderModel::Iterator *, const XAbstractRenderModel *) const;
  virtual xuint32 maxNumberOfPasses(XAbstractCanvas *, const XAbstractRenderModel::Iterator *, const XAbstractRenderModel *) const { return MaxPasses; }
  virtual HitArea hitTest(const QPoint &point, const void *ent, xsize &index) const;
  virtual void move(const QPoint &delta, const void *ent) const;
  virtual void drawConnection(XAbstractCanvas *c, const void *ent, xsize prop, bool fromOutput, const QPoint &to) const;

  struct RenderData
    {
    QPoint position;
    QSize size;
    QRect titleBounds;
    QStaticText title;

    QPoint entOutputPos;
    xuint32 entOutputRadius;

    struct PropertyData
      {
      QStaticText text;
      QPoint position;
      QSize renderSize;
      bool onRight;
      };

    QVector<PropertyData> properties;
    };
  mutable QHash<const void *, RenderData> _renderData;
  void updateRenderData(const SEntity *ent) const;
  void ensureRenderData(const SEntity *ent) const;

private:
  void setupProperty(const QFont& font, RenderData::PropertyData& data, const SProperty *prop) const;

  QFont _titleFnt;
  QFontMetrics _titleFntMetrics;

  QFont _propFnt;
  QFontMetrics _propFntMetrics;
  };

#endif // GCABSTRACTNODEDELEGATE_H
