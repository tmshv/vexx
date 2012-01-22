#ifndef SCNODEDISPLAY_H
#define SCNODEDISPLAY_H

#include "QDeclarativeItem"
#include "QAbstractItemModel"
#include "sentityweakpointer.h"
#include "sobserver.h"

class ScPropertyItem;
class ScNodeItem;
class ScConnectorItem;

class ScNodeDisplay : public QDeclarativeItem, STreeObserver, SConnectionObserver
  {
  Q_OBJECT
  Q_PROPERTY(QDeclarativeComponent *input READ input WRITE setInput)
  Q_PROPERTY(QDeclarativeComponent *output READ output WRITE setOutput)
  Q_PROPERTY(QDeclarativeComponent *node READ node WRITE setNode)
  Q_PROPERTY(QDeclarativeComponent *connector READ connector WRITE setConnector)
  Q_PROPERTY(QObject *model READ model WRITE setModel)
  Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex NOTIFY rootIndexChanged)
  //Q_PROPERTY(QModelIndex topRootIndex READ topRootIndex WRITE setTopRootIndex NOTIFY topRootIndexChanged)

  Q_PROPERTY(int count READ count NOTIFY nodeAdded)

public:
  ScNodeDisplay(QDeclarativeItem *parent = 0);

  void setModel(QObject* );
  QAbstractItemModel* model() const;

  int count() const;
  Q_INVOKABLE ScNodeItem *nodeAt(int i);

  Q_INVOKABLE void setRootToParent();
  Q_INVOKABLE void setRootIndex(QObject *);
  void setRootIndex(const QModelIndex& );
  void setRootIndex(SEntity *);
  QModelIndex rootIndex() const { return QModelIndex(); }

  QDeclarativeComponent *input() const;
  QDeclarativeComponent *output() const;
  QDeclarativeComponent *node() const;
  QDeclarativeComponent *connector() const;

  void setInput(QDeclarativeComponent *);
  void setOutput(QDeclarativeComponent *);
  void setNode(QDeclarativeComponent *);
  void setConnector(QDeclarativeComponent *);

  Q_INVOKABLE void changeItemInput(QDeclarativeItem *item, QDeclarativeItem *newInput);

signals:
  void rootIndexChanged();
  void nodeAdded(ScNodeItem *node);
  //void topRootIndexChanged();

private slots:
  void clear();
  void rebuild();

private:
  void onTreeChange(const SChange *);
  void onConnectionChange(const SChange *);

  enum Mode
    {
    None,
    Input,
    Output
    };
  ScNodeItem *addNode(QDeclarativeComponent *c, SEntity *e, Mode m = None);
  void removeNode(SEntity *e);

  ScPropertyItem *findProperty(const SProperty *, bool driver);

  void addConnectors(SPropertyContainer *);
  void addConnector(const SProperty *);
  void addConnector(const SProperty *dvnProp, const SProperty *dvrProp, ScPropertyItem *driven, ScPropertyItem *driver);

  QDeclarativeComponent *_input;
  QDeclarativeComponent *_output;
  QDeclarativeComponent *_node;
  QDeclarativeComponent *_connector;

  QAbstractItemModel *_model;
  SEntityWeakPointer _rootIndex;

  QVector <ScNodeItem*> _nodes;
  QVector <ScConnectorItem*> _connectors;
  ScNodeItem* _inputItem;
  ScNodeItem* _outputItem;
  };

#endif // SCNODEDISPLAY_H
