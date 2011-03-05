#ifndef NODEGRAPHIC_H
#define NODEGRAPHIC_H

#include "scglobal.h"
#include "QColor"
#include "QFont"
#include "QRectF"

class QVector2D;
class ScNode;
class ScConnection;
class QPainter;
class QPainterPath;

class SC_EXPORT ScNodeGraphic
  {
public:
  virtual ~ScNodeGraphic();
  virtual void draw(QPainter &ptr, const ScNode *data) = 0;


  enum IntersectionMode
    {
    Full,
    Select
    };
  virtual QRectF getBounds(const ScNode *data, IntersectionMode m) = 0;
  virtual bool getShape(const ScNode *data, IntersectionMode m, QPainterPath &path);
  virtual QPointF getConnectionPoint(const ScNode *data, int variable) = 0;
  virtual QVector2D getConnectionNormal(const ScNode *data, int variable) = 0;

  static ScNodeGraphic &getDefault();
  };

class SC_EXPORT ScConnectionGraphic
  {
public:
  virtual ~ScConnectionGraphic();
  virtual void draw(QPainter &ptr, QPointF a, QVector2D norma, QPointF b, QVector2D normb) = 0;

  static ScConnectionGraphic &getDefault();
  };

#endif // NODEGRAPHIC_H
