#ifndef SCNODEITEM_H
#define SCNODEITEM_H

#include "scpropertyitem.h"

class ScNodeItem : public ScPropertyItem
  {
  Q_OBJECT

  Q_PROPERTY(QString specialMode READ specialMode CONSTANT)

public:
  explicit ScNodeItem(QDeclarativeItem *parent = 0);
  
  void setEntity(SEntity *ent, const QString& specialMode);

  Q_INVOKABLE void setPosition(float x, float y);

  virtual const QString& name() const;
  virtual void setName(const QString& );

  QString specialMode() const;

signals:
  
public slots:
  void positionChanged();
  
private:
  QString _specialMode;
  };

#endif // SCNODEITEM_H
