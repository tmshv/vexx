#include "scnode.h"
#include "scnodegraphic.h"

ScNode::Variable::Variable(const QString &t, int ty, QIcon ic) : _name(t), _type(ty), _icon(ic)
  {
  }

ScNode::ScNode() : _isSelected(false), _needsUpdate(true)
  {
  }
ScNode::~ScNode()
  {
  }

void ScNode::setName(QString _in)
  {
  _name = _in;
  _needsUpdate = true;
  }

void ScNode::setSelected(bool _in)
  {
  _isSelected = _in;
  _needsUpdate = true;
  }

QList<ScNode::Variable> &ScNode::variables()
  {
  _needsUpdate = true;
  return _variables;
  }

void ScNode::updated() const
  {
  _needsUpdate = false;
  }

bool ScNode::needsUpdate() const
  {
  return _needsUpdate;
  }

ScNodeGraphic &ScNode::graphic() const
  {
  return ScNodeGraphic::getDefault();
  }

ScConnectionGraphic &ScNode::connectionGraphic() const
  {
  return ScConnectionGraphic::getDefault();
  }
