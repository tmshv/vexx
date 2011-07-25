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

void GCSimpleNodeDelegate::preSetupProperty(const QFont& font, RenderData::PropertyData& data, const SProperty *prop, int yOffset) const
  {
  QFontMetrics propFntMetrics(font);

  QString name = prop->name();
  if(name.isEmpty())
    {
    name = "[" + QString::number(prop->index()) + "]";
    }

  QString elided = propFntMetrics.elidedText(name, Qt::ElideRight, MAX_WIDTH - 2*OUTER_PADDING);
  data.text.setText(elided);
  data.text.prepare(QTransform(), font);

  data.onRight = (prop->hasOutputs() && !prop->hasInput()) || prop->isComputed();

  data.position.setY(yOffset);

  int fillHeight = propFntMetrics.height();
  int fillWidth = propFntMetrics.width(elided);

  yOffset += fillHeight;

  const SPropertyContainer* cont = prop->castTo<SPropertyContainer>();
  if(cont)
    {
    data.childProperties.resize(cont->size());

    int i = 0;
    for(SProperty *child=cont->firstChild(); child; child=cont->nextSibling(), ++i)
      {
      RenderData::PropertyData& childData = data.childProperties[i];

      preSetupProperty(font, childData, child, yOffset);

      fillWidth = qMax(fillWidth, childData.renderSize.width());

      yOffset += childData.renderSize.height();
      fillHeight += childData.renderSize.height();
      }
    }

  data.renderSize.setHeight(fillHeight);
  data.renderSize.setWidth(fillWidth);
  }

void GCSimpleNodeDelegate::postSetupProperty(const QFont& font, RenderData::PropertyData& data, const SProperty *prop, int minX, int maxWidth) const
  {
  const SPropertyContainer* cont = prop->castTo<SPropertyContainer>();
  if(cont)
    {
    int newMinX = minX + OUTER_PADDING;
    int newMaxWidth = maxWidth - 2 * OUTER_PADDING;

    int i = 0;
    for(SProperty *child=cont->firstChild(); child; child=cont->nextSibling(), ++i)
      {
      RenderData::PropertyData& childData = data.childProperties[i];

      postSetupProperty(font, childData, child, newMinX, newMaxWidth);

      if(childData.onRight)
        {
        childData.position.setX(newMaxWidth - childData.renderSize.width());
        }
      else
        {
        childData.position.setX(newMinX);
        }
      }
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

  xsize numProperties = ent->size();

  int fillWidth = titleFntMetrics.width(ent->name());

  int propYStart = _titleFntMetrics.height() + OUTER_PADDING + (2 * TITLE_PADDING) + PROP_PADDING;

  int i = 0;
  rd.properties.resize(numProperties);
  for(SProperty *prop=ent->firstChild(); prop; prop=prop->nextSibling(), ++i)
    {
    RenderData::PropertyData& data = rd.properties[i];

    preSetupProperty(propFnt, data, prop, propYStart);

    data.position.setY(propYStart);
    propYStart += data.renderSize.height();

    fillWidth = qMax(fillWidth, data.renderSize.width());
    }


  fillWidth = qMax(fillWidth, MAX_WIDTH);

  rd.size.setWidth(fillWidth + (2*TITLE_PADDING) + (3*OUTER_PADDING) + titleFntMetrics.height());
  rd.size.setHeight(propYStart + OUTER_PADDING);

  rd.titleBounds.setTopLeft(QPoint(OUTER_PADDING, OUTER_PADDING));
  rd.titleBounds.setBottomRight(QPoint(rd.size.width() - (2*OUTER_PADDING) - 1 - titleFntMetrics.height(),
              _titleFntMetrics.height() + TITLE_PADDING + OUTER_PADDING));

  i = 0;
  for(SProperty *prop=ent->firstChild(); prop; prop=prop->nextSibling(), ++i)
    {
    RenderData::PropertyData& data = rd.properties[i];

    postSetupProperty(propFnt, data, prop, OUTER_PADDING, rd.size.width() - (2*OUTER_PADDING));

    if(data.onRight)
      {
      data.position.setX(rd.size.width() - data.renderSize.width() - (2*OUTER_PADDING));
      }
    else
      {
      data.position.setX(OUTER_PADDING);
      }
    }


  rd.entOutputRadius = rd.titleBounds.height() / 2;
  rd.entOutputPos = QPoint(rd.titleBounds.right() + OUTER_PADDING, OUTER_PADDING);

  rd.title.setText(titleFntMetrics.elidedText(ent->name(), Qt::ElideRight, fillWidth));
  rd.title.prepare(QTransform(), titleFnt);
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

void GCSimpleNodeDelegate::paintProperties(QPainter *ptr, QPoint nodePos, const QVector<RenderData::PropertyData> &props) const
  {
  foreach(const RenderData::PropertyData &prop, props)
    {
    ptr->drawStaticText(nodePos + prop.position, prop.text);

    paintProperties(ptr, nodePos, prop.childProperties);
    }
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
      if(prop->hasInput())
        {
        SProperty *input = prop->input();

        SProperty *inputProp = input;
        SEntity *connectedEnt = 0;
        while(inputProp && !ent)
          {
          inputProp = inputProp->parent();
          if(inputProp == ent->parent())
            {
            connectedEnt = inputProp->castTo<SEntity>();
            }
          }

        if(!ent)
          {
          xAssertFail();
          return;
          }

        ensureRenderData(connectedEnt);
        const RenderData &inputRD(_renderData.value(connectedEnt));

        QPoint inputPosition = rd.position + rd.properties[index].position + QPoint(-OUTER_PADDING, _propFntMetrics.height() / 2);
        QPoint outputPosition;

        if(input == connectedEnt)
          {
          outputPosition = inputRD.position + QPoint(inputRD.size.width(), inputRD.entOutputPos.y() + inputRD.entOutputRadius);
          }
        else
          {
          outputPosition = inputRD.position + inputRD.properties[input->index()].position + QPoint(-OUTER_PADDING, _propFntMetrics.height() / 2);
          }

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

      paintProperties(ptr, rd.position, rd.properties);
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
      xsize i = 0;
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
    connectingPosition = rd.position + rd.properties[prop].position + QPoint(-OUTER_PADDING, _propFntMetrics.height() / 2);
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
