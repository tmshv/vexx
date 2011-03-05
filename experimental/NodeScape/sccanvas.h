#ifndef NODESCAPECANVAS_H
#define NODESCAPECANVAS_H

#include "QWidget"
#include "scglobal.h"
#include "XProperty"
#include "scnodegraphic.h"
#include "QSet"

class ScNode;
class ScConnection;
class QPixmap;

class SC_EXPORT ScCanvas : public QWidget
  {
  Q_OBJECT

public:
  ScCanvas(QWidget *parent = 0);
  ~ScCanvas();

  void select(ScNode *);
  void select(QPointF);
  void select(QRectF);
  QList<int> selection() const;

  void addNode(ScNode *);
  void removeNode(ScNode *);

  void addConnection(ScNode *na, int a, ScNode *nb, int b);
  void removeConnection(ScNode *na, int a, ScNode *nb, int b);

  int intersectNode(QPointF point, ScNodeGraphic::IntersectionMode m) const;

private:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

  void select(int);

  void raise(int);
  void raise(const QList<int> &);

  void drawNode(QPainter &,int);
  void drawConnection(QPainter &, const ScConnection &data);

  QList <ScNode*> _nodes;
  QList <QPixmap> _bakedNodes;

  QSet <ScConnection> _connections;

  enum SelectionMode
    {
    Intersects,
    Contains
    };
  SelectionMode _selectionMode;

  enum MouseMode
    {
    None,
    MoveNode
    };
  MouseMode _mouseMode;
  QPointF _lastMousePosition;
  QPointF _pressMousePosition;
  int _pressNode;
  int _buttonsDown;
  };

#endif // NODESCAPECANVAS_H
