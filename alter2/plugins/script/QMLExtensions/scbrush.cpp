#include "scbrush.h"


void ScPen::setColor(const QColor &c)
  {
  _color = c;
  _valid = (_color.alpha() && _width >= 1) ? true : false;
  emit penChanged();
  }

void ScPen::setWidth(int w)
  {
  if(_width == w && _valid)
    {
    return;
    }

  _width = w;
  _valid = (_color.alpha() && _width >= 1) ? true : false;
  emit penChanged();
  }

void ScGradientStop::updateGradient()
  {
  if(ScGradient *grad = qobject_cast<ScGradient*>(parent()))
    {
    grad->doUpdate();
    }
  }

const QGradient *ScGradient::gradient() const
  {
  if(!m_gradient && !m_stops.isEmpty())
    {
    m_gradient = new QLinearGradient(0,0,0,1.0);
    for (int i = 0; i < m_stops.count(); ++i)
      {
      const ScGradientStop *stop = m_stops.at(i);
      m_gradient->setCoordinateMode(QGradient::ObjectBoundingMode);
      m_gradient->setColorAt(stop->position(), stop->color());
      }
    }

  return m_gradient;
  }

void ScGradient::doUpdate()
  {
  delete m_gradient;
  m_gradient = 0;
  emit updated();
  }
