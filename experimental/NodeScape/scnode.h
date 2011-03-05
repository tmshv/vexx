#ifndef NODEDATA_H
#define NODEDATA_H

#include "scglobal.h"
#include "XProperty"
#include "QString"
#include "QPointF"
#include "QList"
#include "QIcon"

class ScNodeGraphic;
class ScConnectionGraphic;

class SC_EXPORT ScNode
  {
public:
  class Variable
    {
  public:
    Variable(const QString &t, int ty, QIcon ic=QIcon());

  properties:
      XProperty(QString, name, setName);
      XProperty(int, type, setType);
      XProperty(QIcon, icon, setIcon);
    };

  ScNode();
  virtual ~ScNode();

  void setName(QString _in);
  void setSelected(bool _in);
  QList<Variable> &variables();
  void updated() const;
  bool needsUpdate() const;

  virtual ScNodeGraphic &graphic() const;
  virtual ScConnectionGraphic &connectionGraphic() const;

properties:
  XProperty(QPointF, position, setPosition);
  XROProperty(QString, name);
  XROProperty(bool, isSelected);
  XRORefProperty(QList<Variable>, variables);

private:
  mutable bool _needsUpdate;
  };

#endif // NODEDATA_H
