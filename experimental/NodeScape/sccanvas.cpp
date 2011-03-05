#include "sccanvas.h"
#include "QPainter"
#include "QMouseEvent"
#include "scnodegraphic.h"
#include "scconnection.h"
#include "scnode.h"
#include "QVector2D"
#include "QDebug"

ScCanvas::ScCanvas(QWidget *parent)
    : QWidget(parent),
    _selectionMode(Intersects),
    _mouseMode(None)
  {
  }

ScCanvas::~ScCanvas()
  {
  }

void ScCanvas::paintEvent(QPaintEvent *)
  {
  QPainter ptr(this);

  ptr.setRenderHint(QPainter::Antialiasing, true);
  foreach(const ScConnection &data, _connections)
    {
    drawConnection(ptr, data);
    }
  ptr.setRenderHint(QPainter::Antialiasing, false);

  for(int id=0; id<_nodes.size(); ++id)
    {
    drawNode(ptr, id);
    }

  if(_pressNode == -1 && (_buttonsDown&Qt::LeftButton) != false)
    {
    QColor selection(Qt::darkGray);
    ptr.setPen(selection);
    selection.setAlpha(50);
    ptr.setBrush(selection);
    QRectF rct;
    rct.setTopLeft(_pressMousePosition);
    rct.setBottomRight(_lastMousePosition);
    ptr.drawRect(rct);
    }
  }

void ScCanvas::drawNode(QPainter &canvas, int id)
  {
  xAssert(id<_nodes.size());
  xAssert(id<_bakedNodes.size());

  ScNode *data = _nodes[id];

  if(data->needsUpdate())
    {
    data->updated();

    QRectF rct(data->graphic().getBounds(data,ScNodeGraphic::Full));
    QPixmap pix(rct.size().toSize());
    pix.fill(Qt::transparent);
    QPainter ptr(&pix);
    ptr.setRenderHint(QPainter::Antialiasing);
    ptr.translate(-rct.topLeft());
    data->graphic().draw(ptr, data);
    _bakedNodes[id] = pix;
    }

  canvas.drawPixmap(data->position(), _bakedNodes[id]);
  }

void ScCanvas::drawConnection(QPainter &canvas, const ScConnection &data)
  {
  QPointF pntA = data.nodeA()->position() + data.nodeA()->graphic().getConnectionPoint(data.nodeA(), data.variableA());
  QPointF pntB = data.nodeB()->position() + data.nodeB()->graphic().getConnectionPoint(data.nodeB(), data.variableB());


  QVector2D normA = data.nodeA()->graphic().getConnectionNormal(data.nodeA(), data.variableA());
  QVector2D normB = data.nodeB()->graphic().getConnectionNormal(data.nodeB(), data.variableB());

  data.nodeA()->connectionGraphic().draw(canvas, pntA, normA, pntB, normB);
  }

void ScCanvas::mousePressEvent(QMouseEvent *event)
  {
  _buttonsDown = event->buttons();
  _pressNode = intersectNode(event->pos(),ScNodeGraphic::Select);
  _lastMousePosition = _pressMousePosition = event->pos();
  }

void ScCanvas::mouseMoveEvent(QMouseEvent *event)
  {
  _buttonsDown = event->buttons();

  QList<int> sel = selection();

  if(!sel.contains(_pressNode))
    {
    select(_pressNode);
    }

  sel = selection();
  foreach(const int &s, sel)
    {
    ScNode *node(_nodes[s]);
    node->setPosition(node->position() + event->pos() - _lastMousePosition);
    }

  _lastMousePosition = event->pos();
  update();
  }

void ScCanvas::mouseReleaseEvent(QMouseEvent *event)
  {
  if(_pressNode == -1 && (_buttonsDown&Qt::LeftButton) != false)
    {
    QRectF rct;
    rct.setTopLeft(_pressMousePosition);
    rct.setBottomRight(_lastMousePosition);
    select(rct);
    }
  
  _buttonsDown = event->buttons();
  // point select
  if(event->pos() == _pressMousePosition)
    {
    select(event->pos());
    }

  _lastMousePosition = event->pos();
  update();
  _pressNode = -1;
  }

int ScCanvas::intersectNode(QPointF point, ScNodeGraphic::IntersectionMode m) const
  {
  int selected = -1;
  for(int id=0; id<_nodes.size(); ++id)
    {
    const ScNode *node(_nodes[id]);

    QRectF rct(node->graphic().getBounds(node,m));

    rct.moveTopLeft(node->position().toPoint());
    if(rct.contains(point))
      {
      selected = id;
      }
    }
  return selected;
  }

void ScCanvas::select(int select)
  {
  for(int id=0; id<_nodes.size(); ++id)
    {
    ScNode *node(_nodes[id]);
    node->setSelected(false);
    }
  if(select != -1)
    {
    _nodes[select]->setSelected(true);
    raise(select);
    }
  }

void ScCanvas::select(QPointF pnt)
  {
  select(intersectNode(pnt,ScNodeGraphic::Select));
  }

QList<int> ScCanvas::selection() const
  {
  QList<int> sel;
  for(int id=0; id<_nodes.size(); ++id)
    {
    const ScNode *node(_nodes[id]);
    bool wasSelected = node->isSelected();
    if(wasSelected != false)
      {
      sel << id;
      }
    }
  return sel;
  }

void ScCanvas::select(QRectF drag)
  {
  QList<int> sel;
  for(int id=0; id<_nodes.size(); ++id)
    {
    ScNode *node(_nodes[id]);
    QRectF rct(node->graphic().getBounds(node,ScNodeGraphic::Full));
    rct.moveTopLeft(node->position().toPoint());
    if((_selectionMode == Intersects && rct.intersects(drag)) || (_selectionMode == Contains && rct.contains(drag)))
      {
      sel << id;
      }
    node->setSelected(false);
    }

  foreach(const int &s, sel)
    {
    ScNode *node(_nodes[s]);
    node->setSelected(true);
    }
  raise(sel);
  }

void ScCanvas::raise(int i)
  {
  _nodes.swap(i, _nodes.size()-1);
  _bakedNodes.swap(i, _nodes.size()-1);

  if(_pressNode == i)
    {
    _pressNode = _nodes.size()-1;
    }
  }

void ScCanvas::raise(const QList<int> &in)
  {
  foreach(const int &i, in)
    {
    raise(i);
    }
  }

void ScCanvas::addNode(ScNode *n)
  {
  _nodes << n;
  _bakedNodes << QPixmap();
  }

void ScCanvas::removeNode(ScNode *n)
  {
  int i = _nodes.indexOf(n);
  _nodes.removeAt(i);
  _bakedNodes.removeAt(i);
  }

void ScCanvas::addConnection(ScNode *na, int a, ScNode *nb, int b)
  {
  _connections << ScConnection(na, a, nb, b);
  }

void ScCanvas::removeConnection(ScNode *na, int a, ScNode *nb, int b)
  {
  _connections.remove( ScConnection(na, a, nb, b) );
  }
