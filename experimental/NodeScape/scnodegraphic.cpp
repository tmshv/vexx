#include "scnodegraphic.h"
#include "scnode.h"
#include "QIcon"
#include "QVector2D"
#include "QPainter"
#include "QDebug"

class ScBasicNodeGraphic : public ScNodeGraphic
  {
public:
  ScBasicNodeGraphic();
  void draw(QPainter &ptr, const ScNode *data);
  QRectF getBounds(const ScNode *data, IntersectionMode);

  QPointF getConnectionPoint(const ScNode *data, int variable);
  QVector2D getConnectionNormal(const ScNode *data, int variable);

private:
  xReal nodeWidth;
  xReal radius;
  xReal outlineWidth;
  xReal nodeOutlineFade;
  xReal outerOffset;
  xReal titleTextHeightPadding;
  xReal titleTextLeftPadding;
  xReal variableWidthOffset;
  xReal iconPaddingWidth;
  xReal iconHeightAdjust;
  QColor basic;
  QColor selected;
  QFont titleFont;
  QFont variableFont;
  qreal variableHeight;
  qreal variablePadding;
  };

class ScBasicConnectionGraphic : public ScConnectionGraphic
  {
public:
  ScBasicConnectionGraphic();
  void draw(QPainter &ptr, QPointF, QVector2D, QPointF, QVector2D);

private:
  qreal cubicMultiplier;
  };

ScNodeGraphic::~ScNodeGraphic()
  {
  }

ScNodeGraphic &ScNodeGraphic::getDefault()
  {
  static ScBasicNodeGraphic sc;
  return sc;
  }

ScBasicNodeGraphic::ScBasicNodeGraphic()
  {
  nodeWidth = 150.0f;
  radius = 5.0f;
  outlineWidth = 1.0f;
  nodeOutlineFade = 0.75;
  outerOffset = 3.0f;
  titleTextHeightPadding = 2.0f;
  titleTextLeftPadding = 2.0f;
  variableWidthOffset = 10.0f;
  iconPaddingWidth = 2.0f;
  iconHeightAdjust = -1.0f;

  selected = QColor(240, 210, 0);
  basic = QColor(212, 212, 212);

  titleFont.setPixelSize(20);
  variableFont.setPixelSize(10);

  variableHeight = 10.0f;
  variablePadding = 2.0f;
  }

QRectF ScBasicNodeGraphic::getBounds(const ScNode *data, IntersectionMode m)
  {
  qreal titleTextHeight = titleFont.pixelSize();
  qreal headerHeight = titleTextHeight + ((titleTextLeftPadding+outerOffset)*2);
  qreal nodeHeight = headerHeight +
                     (variableHeight * data->variables().size()) +
                     (variablePadding * (data->variables().size()-1)) +
                     outerOffset;

  if(m == ScNodeGraphic::Select)
    {
    QRectF rct(QPointF(0,0),QSizeF(nodeWidth,headerHeight));
    return rct.adjusted(-outlineWidth,-outlineWidth,outlineWidth*2,outlineWidth*2);
    }

  QRectF rct(QPointF(0,0),QSizeF(nodeWidth,nodeHeight));
  return rct.adjusted(-outlineWidth,-outlineWidth,outlineWidth*2,outlineWidth*2);
  }

void ScBasicNodeGraphic::draw(QPainter &ptr, const ScNode *data)
{
  QColor nodeColour(basic);
  if(data->isSelected())
    {
    nodeColour = selected;
    }

  // auto calculated sizes
  qreal titleTextHeight = titleFont.pixelSize();
  qreal headerHeight = titleTextHeight + ((titleTextLeftPadding+outerOffset)*2);
  qreal nodeHeight = headerHeight +
                     (variableHeight * data->variables().size()) +
                     (variablePadding * (data->variables().size()-1)) +
                     outerOffset;
  QSize iconSize(variableWidthOffset-iconPaddingWidth,variableWidthOffset-iconPaddingWidth);
  float variableSectionHeight = variableHeight+variablePadding;


  // colour information
  QColor faintOutline(nodeColour.lighter());
  faintOutline.setAlpha(faintOutline.alpha() * nodeOutlineFade);
  QPen nodeLine(faintOutline,outlineWidth);
  QLinearGradient nodeGradient(QPoint(0,0), QPoint(0,nodeHeight));
  nodeGradient.setColorAt(1.0f, nodeColour);
  nodeGradient.setColorAt(0.0f, nodeColour.darker(125));
  QBrush nodeBrush(nodeGradient);

  QPen titleText(Qt::black);
  QPen variableText(Qt::black);

  QPen titleLine(Qt::transparent);
  QBrush titleBrush(nodeColour);

  // draw
  ptr.setPen(nodeLine);
  ptr.setBrush(nodeBrush);
  ptr.drawRoundedRect(QRect(QPoint(0,0),QSize(nodeWidth,nodeHeight)), radius, radius);

  ptr.setPen(titleLine);
  ptr.setBrush(titleBrush);
  QRect titleRect(QPoint(outerOffset,outerOffset),
                  QSize(nodeWidth-2*outerOffset, titleTextHeight+(2*titleTextHeightPadding)));
  ptr.setClipRect(titleRect);
  ptr.setClipping(true);
  ptr.drawRoundedRect(titleRect.adjusted(0,0,0,2*outerOffset), radius-outerOffset, radius-outerOffset);
  ptr.setClipping(false);

  ptr.setFont(titleFont);
  ptr.setPen(titleText);
  ptr.setBrush(Qt::transparent);
  QString text(QFontMetrics(titleFont).elidedText(data->name(),Qt::ElideRight,titleRect.width()));
  ptr.drawText(titleRect.adjusted(titleTextLeftPadding,-outerOffset,0,0), Qt::AlignVCenter, text);

  ptr.setFont(variableFont);
  ptr.setPen(variableText);
  ptr.translate(0,headerHeight);
  QFontMetrics fntMet(variableFont);
  foreach(const ScNode::Variable &var, data->variables())
  {
    int flags = Qt::AlignVCenter|Qt::TextDontClip;
    QRect iconRect = QRect(QPoint(0,0), iconSize);
    iconRect.moveTop(((variableSectionHeight-iconSize.height())/2)+iconHeightAdjust);
    QRect variabltTextRect(outerOffset+variableWidthOffset,
                           -0.5*variablePadding,
                           nodeWidth-((variableWidthOffset+outerOffset)*2),
                           variableHeight);

    if(var.type() == 1)
      {
      flags |= Qt::AlignRight;
      iconRect.moveLeft(nodeWidth-(iconSize.width()+outerOffset));
      text = fntMet.elidedText(var.name(),Qt::ElideLeft,variabltTextRect.width());
      }
    else
      {
      iconRect.moveLeft(outerOffset);
      text = fntMet.elidedText(var.name(),Qt::ElideRight,variabltTextRect.width());
      }

    ptr.drawText(variabltTextRect, flags, text);

    ptr.drawPixmap(iconRect, var.icon().pixmap(iconRect.size(), data->isSelected()?QIcon::Active:QIcon::Disabled, QIcon::On));

    ptr.translate(0,variableSectionHeight);
  }
}

QPointF ScBasicNodeGraphic::getConnectionPoint(const ScNode *data, int variable)
  {
  qreal titleTextHeight = titleFont.pixelSize();
  qreal headerHeight = titleTextHeight + ((titleTextLeftPadding+outerOffset)*2);
  float variableSectionHeight = variableHeight+variablePadding;
  float height = headerHeight + (((float)variable+0.5f) * variableSectionHeight);

  const ScNode::Variable &var = data->variables()[variable];
  if(var.type() == 1)
    {
    return QPointF(nodeWidth,height);
    }
  return QPointF(0.0f,height);
  }

QVector2D ScBasicNodeGraphic::getConnectionNormal(const ScNode *data, int variable)
  {
  const ScNode::Variable &var = data->variables()[variable];
  if(var.type() == 1)
    {
    return QVector2D(1.0f,0.0f);
    }
  return QVector2D(-1.0f,0.0f);
  }

ScBasicConnectionGraphic::ScBasicConnectionGraphic()
  {
  cubicMultiplier = 50.0f;
  }

void ScBasicConnectionGraphic::draw(QPainter &ptr, QPointF a, QVector2D norma, QPointF b, QVector2D normb)
  {
  float headDist = -10.0f;
  if(normb.x() > 0.0f)
    {
    headDist = 10.0f;
    }

  QPolygonF arrow;
  arrow.append(b);
  arrow.append(b+QPointF(headDist,-3));
  arrow.append(b+QPointF(headDist,3));
  arrow.append(b);

  b += QPointF(headDist,0);
  QPainterPath p;
  p.moveTo(a);
  p.cubicTo(a+(cubicMultiplier*norma).toPointF(),b+(cubicMultiplier*normb).toPointF(),b);

  QPainterPath ends;
  ends.addEllipse(a, 3, 3);
  ends.addPolygon(arrow);

  QPen connPen(Qt::black);
  QBrush connBrush(Qt::black);

  ptr.setPen(connPen);
  ptr.setBrush(connBrush);
  ptr.drawPath(ends);

  connPen.setWidthF(2.5);
  ptr.setPen(connPen);
  ptr.setBrush(Qt::transparent);
  ptr.drawPath(p);
  }

bool ScNodeGraphic::getShape(const ScNode *data, IntersectionMode m, QPainterPath &path)
  {
  return false;
  }

ScConnectionGraphic::~ScConnectionGraphic()
  {
  }

ScConnectionGraphic &ScConnectionGraphic::getDefault()
  {
  static ScBasicConnectionGraphic con;
  return con;
  }
