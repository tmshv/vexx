#include "GCAbstractNodeDelegate.h"
#include "GCShiftRenderModel.h"
#include "X2DCanvas.h"
#include "QPen"
#include "QPainter"
#include "sentity.h"

#define MAX_WIDTH 128
#define OUTER_PADDING 5
#define TITLE_PADDING 2
#define PROP_PADDING 2
#define SHADOW_OFFSET 3
#define CONNECTION_EXTENSION 50

#define NUM_PROPERTIES_T0_SKIP 0

GCSimpleNodeDelegate::GCSimpleNodeDelegate() : _titleFntMetrics(_titleFnt), _propFntMetrics(_propFnt)
  {
  _titleFnt.setBold(true);
  _titleFntMetrics = QFontMetrics(_titleFnt);

  _propFnt.setPixelSize(_propFnt.pixelSize() * 0.7f);
  _propFntMetrics = QFontMetrics(_titleFnt);
  }


void GCSimpleNodeDelegate::ensureRenderData(const SEntity *ent) const
  {
  if(!_renderData.contains(ent))
    {
    updateRenderData(ent);
    }
  }

void GCSimpleNodeDelegate::updateRenderData(const SEntity *ent) const
  {
  xAssert(ent);

  if(!_renderData.contains(ent))
    {
    RenderData rd;
    rd.position.setX(100);
    rd.position.setY(100);
    _renderData[ent] = rd;
    }

  RenderData &rd(_renderData[ent]);

  QFont titleFnt;
  titleFnt.setBold(true);
  titleFnt.setPixelSize(11);
  QFontMetrics titleFntMetrics(titleFnt);

  QFont propFnt;
  propFnt.setPixelSize(9);
  QFontMetrics propFntMetrics(titleFnt);

  xsize numProperties = ent->size() - NUM_PROPERTIES_T0_SKIP;

  int fillWidth = titleFntMetrics.width(ent->name());

  size_t i = 0;
  for(SProperty *prop=ent->firstChild(); prop; prop=prop->nextSibling(), ++i)
    {
    if(i<NUM_PROPERTIES_T0_SKIP)
      {
      continue;
      }
    fillWidth = qMax(fillWidth, titleFntMetrics.width(prop->name()));
    }
  fillWidth = qMin(fillWidth, MAX_WIDTH);

  float innerWidth = fillWidth + (2*TITLE_PADDING) + OUTER_PADDING;

  rd.size.setWidth(fillWidth + (2*TITLE_PADDING) + (3*OUTER_PADDING) + titleFntMetrics.height());
  rd.size.setHeight(titleFntMetrics.height() + ((TITLE_PADDING + OUTER_PADDING) * 2) + (numProperties * (PROP_PADDING + propFntMetrics.height())));

  rd.titleBounds.setTopLeft(QPoint(OUTER_PADDING, OUTER_PADDING));
  rd.titleBounds.setBottomRight(QPoint(rd.size.width() - (2*OUTER_PADDING) - 1 - titleFntMetrics.height(),
              _titleFntMetrics.height() + TITLE_PADDING + OUTER_PADDING));

  rd.entOutputRadius = rd.titleBounds.height() / 2;
  rd.entOutputPos = QPoint(rd.titleBounds.right() + OUTER_PADDING, OUTER_PADDING);

  rd.title.setText(titleFntMetrics.elidedText(ent->name(), Qt::ElideRight, fillWidth));
  rd.title.prepare(QTransform(), titleFnt);

  int propYOffset = PROP_PADDING + _propFntMetrics.height();
  int propYStart = _titleFntMetrics.height() + OUTER_PADDING + (2 * TITLE_PADDING) + PROP_PADDING;

  i = 0;
  rd.properties.resize(numProperties);
  for(SProperty *prop=ent->firstChild(); prop; prop=prop->nextSibling(), ++i)
    {
    if(i<NUM_PROPERTIES_T0_SKIP)
      {
      continue;
      }
    QString text = propFntMetrics.elidedText(prop->name(), Qt::ElideRight, innerWidth);
    rd.properties[i].text.setText(text);
    rd.properties[i].text.prepare(QTransform(), propFnt);

    bool shouldAlignRight = (prop->hasOutputs() && !prop->hasInput()) || prop->isComputed();

    rd.properties[i].position.setY(propYStart + i * propYOffset);

    if(shouldAlignRight)
      {
      rd.properties[i].position.setX(rd.size.width() - _propFntMetrics.width(text) - OUTER_PADDING);
      }
    else
      {
      rd.properties[i].position.setX(OUTER_PADDING);
      }
    }
  }

void GCSimpleNodeDelegate::update(const XAbstractCanvas *,
                    const XAbstractRenderModel::Iterator *aIt,
                    const XAbstractRenderModel *) const
  {
  const GCShiftRenderModel::Iterator *it = static_cast<const GCShiftRenderModel::Iterator *>(aIt);
  const SEntity *ent = it->entity();
  xAssert(ent);

  updateRenderData(ent);
  }

void GCSimpleNodeDelegate::paint(xuint32 pass,
                                XAbstractCanvas *c,
                                const XAbstractRenderModel::Iterator *aIt,
                                const XAbstractRenderModel *) const
  {
  const GCShiftRenderModel::Iterator *it = static_cast<const GCShiftRenderModel::Iterator *>(aIt);
  const SEntity *ent = it->entity();
  xAssert(ent);

  ensureRenderData(ent);
  const RenderData &rd(_renderData.value(ent));

  X2DCanvas* canvas = static_cast<X2DCanvas*>(c);
  QPainter *ptr = canvas->currentPainter();
  xAssert(ptr);
  if(pass == ConnectionPass)
    {
    xsize index = 0;
    for(SProperty *prop=ent->firstChild(); prop; prop=prop->nextSibling(), ++index)
      {
      if(index<NUM_PROPERTIES_T0_SKIP)
        {
        continue;
        }

      if(prop->hasInput())
        {
        SProperty *input = prop->input();
        ensureRenderData(input->entity());
        const RenderData &inputRD(_renderData.value(input->entity()));

        QPoint inputPosition = rd.position + rd.properties[index].position + QPoint(-OUTER_PADDING, _propFntMetrics.height() / 2);
        QPoint outputPosition = inputRD.position + inputRD.properties[input->index()-NUM_PROPERTIES_T0_SKIP].position + QPoint(-OUTER_PADDING, _propFntMetrics.height() / 2);

        inputPosition.setX(rd.position.x());
        outputPosition.setX(inputRD.position.x() + inputRD.size.width());

        QPen pen;
        pen.setColor(Qt::white);
        pen.setWidth(3);
        ptr->setPen(pen);
        ptr->setBrush(Qt::transparent);

        QPainterPath path;
        path.moveTo(outputPosition);
        path.cubicTo(outputPosition + QPoint(CONNECTION_EXTENSION, 0),
                     inputPosition - QPoint(CONNECTION_EXTENSION, 0),
                     inputPosition);
        ptr->drawPath(path);
        }
      }
    }
  else if(pass == ShadowPass)
    {
    QRect shadowRect(rd.position, rd.size);
    shadowRect.moveTopLeft(shadowRect.topLeft() + QPoint(SHADOW_OFFSET, SHADOW_OFFSET));

    if(canvas->region().intersects(shadowRect))
      {
      ptr->setBrush(QColor(0, 0, 0, 64));
      ptr->setPen(Qt::transparent);
      ptr->drawRoundedRect(shadowRect, 4, 4);
      }
    }
  else if(pass == NodePass)
    {
    QRect bounds(rd.position, rd.size);
    if(canvas->region().intersects(bounds))
      {
      QPoint renderPoint(rd.position);
      renderPoint += QPoint(OUTER_PADDING + TITLE_PADDING, OUTER_PADDING + TITLE_PADDING);

      ptr->setBrush(QColor(64, 64, 64));
      ptr->setPen(QColor(96, 96, 96));
      ptr->drawRoundedRect(bounds, 4, 4);

      ptr->setBrush(QColor(128, 128, 128));
      ptr->setPen(Qt::transparent);
      ptr->drawRoundedRect(QRect(rd.position + rd.titleBounds.topLeft(), rd.titleBounds.size()), 2, 2);

      ptr->setPen(QColor(96, 96, 96));
      ptr->setBrush(QColor(128, 128, 128));
      ptr->drawEllipse(renderPoint + rd.entOutputPos, rd.entOutputRadius, rd.entOutputRadius);

      ptr->setPen(Qt::black);
      ptr->setFont(_titleFnt);
      ptr->drawStaticText(renderPoint, rd.title);

      ptr->setPen(QColor(196, 196, 196));
      ptr->setFont(_propFnt);
      foreach(const RenderData::PropertyData &prop, rd.properties)
        {
        ptr->drawStaticText(rd.position + prop.position, prop.text);
        }
      }
    }
  }

GCAbstractNodeDelegate::HitArea GCSimpleNodeDelegate::hitTest(const QPoint &point, const void *ent, xsize &index) const
  {
  HitArea area = None;
  xAssert(_renderData.contains(ent));
  const RenderData &rd(_renderData[ent]);

  if(QRect(rd.position, rd.size).contains(point))
    {
    area = NodeDull;

    if(QRect(rd.titleBounds.topLeft() + rd.position, rd.titleBounds.size()).contains(point))
      {
      area = NodeTranslatable;
      }
    else if(QRect(rd.entOutputPos + rd.position, QSize(rd.entOutputRadius*2, rd.entOutputRadius*2)).contains(point))
      {
      area = NodeOutput;
      }
    else
      {
      xsize i = NUM_PROPERTIES_T0_SKIP;
      foreach(const RenderData::PropertyData &prop, rd.properties)
        {
        QRect left(rd.position + prop.position, QSize(rd.size.width() / 2, _propFntMetrics.height()));
        left.moveLeft(rd.position.x());
        QRect right(left);
        right.moveLeft(right.left() + (rd.size.width() / 2));

        if(left.contains(point))
          {
          area = Input;
          index = i;
          }
        else if(right.contains(point))
          {
          area = Output;
          index = i;
          }
        ++i;
        }
      }
    }

  return area;
  }

void GCSimpleNodeDelegate::move(const QPoint &pos, const void *ent) const
  {
  RenderData &rd(_renderData[ent]);
  rd.position += pos;
  }

void GCSimpleNodeDelegate::drawConnection(XAbstractCanvas *c, const void *ent, xsize prop, bool fromOutput, const QPoint &to) const
  {
  X2DCanvas* canvas = static_cast<X2DCanvas*>(c);
  QPainter *ptr = canvas->currentPainter();
  xAssert(ptr);

  const RenderData &rd(_renderData[ent]);

  QPoint connectingPosition;
  if(prop == X_SIZE_SENTINEL)
    {
    connectingPosition = rd.position + rd.entOutputPos + QPoint(rd.entOutputRadius, rd.entOutputRadius);
    }
  else
    {
    connectingPosition = rd.position + rd.properties[prop-NUM_PROPERTIES_T0_SKIP].position + QPoint(-OUTER_PADDING, _propFntMetrics.height() / 2);
    }

  QPainterPath path;
  if(fromOutput)
    {
    connectingPosition.setX(rd.position.x() + rd.size.width());

    path.moveTo(connectingPosition);
    path.cubicTo(connectingPosition + QPoint(CONNECTION_EXTENSION, 0),
                 to - QPoint(CONNECTION_EXTENSION, 0),
                 to);
    }
  else
    {
    connectingPosition.setX(rd.position.x());

    path.moveTo(connectingPosition);
    path.cubicTo(connectingPosition - QPoint(CONNECTION_EXTENSION, 0),
                 to + QPoint(CONNECTION_EXTENSION, 0),
                 to);
    }

  QPen pen;
  pen.setColor(Qt::white);
  pen.setWidth(3);
  ptr->setPen(pen);
  ptr->setBrush(Qt::transparent);

  ptr->drawPath(path);
  }
