#include "scnodedisplay.h"
#include "scnodeitem.h"
#include "scconnectoritem.h"
#include "sentity.h"
#include "XAssert"

ScNodeDisplay::ScNodeDisplay(QDeclarativeItem *parent) : QDeclarativeItem(parent)
  {
  _model = 0;
  _input = 0;
  _output = 0;
  _node = 0;
  _connector = 0;
  }

void ScNodeDisplay::setModel(QObject* m)
  {
  _model = qobject_cast<QAbstractItemModel*>(m);
  rebuild();
  }

QAbstractItemModel* ScNodeDisplay::model() const
  {
  return _model;
  }

void ScNodeDisplay::onTreeChange(const SChange *c)
  {
  if(!_rootIndex)
    {
    return;
    }

  const SPropertyContainer::TreeChange *tC = c->castTo<SPropertyContainer::TreeChange>();
  if(tC)
    {
    SEntity *ent = tC->property()->castTo<SEntity>();
    if(ent)
      {
      if(tC->before() == &_rootIndex->children)
        {
        removeNode(ent);
        return;
        }
      if(tC->after() == &_rootIndex->children)
        {
        addNode(_node, ent);
        return;
        }

      // not caught, try to pass to children
      bool added = false;
      bool removed = false;
      foreach(ScNodeItem *i, _nodes)
        {
        SProperty *p = i->property();
        if(p->isDescendedFrom(tC->before()))
          {
          removed |= i->onPropertyRemoved(tC->before(), p);
          }
        if(p->isDescendedFrom(tC->after()))
          {
          added |= i->onPropertyAdded(p);
          }

        if(added && removed)
          {
          break;
          }
        }
      }
    }
  }

void ScNodeDisplay::setRootToParent()
  {
  setRootIndex(_rootIndex->parentEntity());
  }

void ScNodeDisplay::setRootIndex(QObject *itemToMatch)
  {
  xAssert(itemToMatch);
  for(int i = 0, s = _nodes.size(); i < s; ++i)
    {
    ScNodeItem *item = _nodes[i];
    if(item == itemToMatch)
      {
      setRootIndex(item->property()->entity());
      return;
      }
    }
  xAssertFail();
  }

void ScNodeDisplay::setRootIndex(const QModelIndex &i)
  {
  SProperty *p = (SProperty*)i.internalPointer();
  setRootIndex(p->entity());
  }

void ScNodeDisplay::setRootIndex(SEntity *i)
  {
  if(_rootIndex)
    {
    _rootIndex->removeTreeObserver(this);
    }

  _rootIndex = i;
  emit rootIndexChanged();

  if(_rootIndex)
    {
    _rootIndex->addTreeObserver(this);
    }

  rebuild();
  }

QDeclarativeComponent *ScNodeDisplay::input() const
  {
  return _input;
  }

QDeclarativeComponent *ScNodeDisplay::output() const
  {
  return _output;
  }

QDeclarativeComponent *ScNodeDisplay::node() const
  {
  return _node;
  }

QDeclarativeComponent *ScNodeDisplay::connector() const
  {
  return _connector;
  }

void ScNodeDisplay::setInput(QDeclarativeComponent *i)
  {
  _input = i;
  rebuild();
  }

void ScNodeDisplay::setOutput(QDeclarativeComponent *o)
  {
  _output = o;
  rebuild();
  }

void ScNodeDisplay::setNode(QDeclarativeComponent *n)
  {
  _node = n;
  rebuild();
  }

void ScNodeDisplay::setConnector(QDeclarativeComponent *c)
  {
  _connector = c;
  rebuild();
  }

void ScNodeDisplay::clear()
  {
  foreach(ScConnectorItem *i, _connectors)
    {
    i->deleteLater();
    }

  _connectors.clear();

  foreach(ScNodeItem *i, _nodes)
    {
    i->deleteLater();
    }

  _nodes.clear();

  if(_inputItem)
    {
    _inputItem->deleteLater();
    _inputItem = 0;
    }

  if(_outputItem)
    {
    _outputItem->deleteLater();
    _outputItem = 0;
    }
  }

void ScNodeDisplay::rebuild()
  {
  clear();

  if(!_node)
    {
    return;
    }

  SEntity *e = _rootIndex;
  if(!e)
    {
    return;
    }

  for(SEntity *c = e->children.firstChild<SEntity>(); c; c = c->nextSibling<SEntity>())
    {
    addNode(_node, c);
    }

  if(_rootIndex->parent())
    {
    if(_input)
      {
      _inputItem = addNode(_input, e, Input);
      }

    if(_output)
      {
      _outputItem = addNode(_output, e, Output);
      }
    }

  addConnectors(e);
  }

ScNodeItem *ScNodeDisplay::addNode(QDeclarativeComponent *c, SEntity *e, Mode m)
  {
  xAssert(c);
  xAssert(e);
  QObject *created = c->beginCreate(c->creationContext());
  if(!created)
    {
    qWarning() << c->errorString();
    return 0;
    }

  ScNodeItem* node = qobject_cast<ScNodeItem*>(created);
  xAssert(node);
  if(!node)
    {
    delete created;
    qWarning() << "node property of NodeDisplay must contain a NodeItem at highest level";
    return 0;
    }

  _nodes << node;
  node->setParentItem(this);

  QString mode;
  if(m == Input)
    {
    mode = "input";
    }
  else if(m == Output)
    {
    mode = "output";
    }
  node->setEntity(e, mode);

  c->completeCreate();

  emit nodeAdded(node);

  return node;
  }

void ScNodeDisplay::removeNode(SEntity *e)
  {
  xAssert(e);
  for(int i = 0, s = _nodes.size(); i < s; ++i)
    {
    ScNodeItem *item = _nodes[i];
    if(item->property() == e)
      {
      item->deleteLater();
      _nodes.erase(_nodes.begin() + i);
      return;
      }
    }
  xAssertFail();
  }

ScPropertyItem *ScNodeDisplay::findProperty(SProperty *p, bool driver)
  {
  xAssert(_rootIndex.isValid());
  if(p->parent() == &_rootIndex->children)
    {
    for(int i = 0, s = _nodes.size(); i < s; ++i)
      {
      ScNodeItem *n = _nodes[i];
      if(p->isDescendedFrom(n->property()))
        {
        return n;
        }
      }
    return 0;
    }
  else if(p == _rootIndex)
    {
    if(driver)
      {
      return _inputItem;
      }
    return _outputItem;
    }

  if(!p->parent())
    {
      return 0;
    }

  ScPropertyItem *pItem = findProperty(p->parent(), driver);
  if(!pItem)
    {
    return 0;
    }

  ScPropertyDisplay *display = pItem->display();
  if(!display)
    {
    return pItem;
    }

  const QVector<ScPropertyItem *> props = display->properties();
  for(int i = 0, s = props.size(); i < s; ++i)
    {
    ScPropertyItem *n = props[i];
    if(p->isDescendedFrom(n->property()))
      {
      return n;
      }
    }

  return 0;
  }

void ScNodeDisplay::addConnectors(SPropertyContainer *e)
  {
  for(SProperty *c=e->firstChild(); c; c=c->nextSibling())
    {
    SProperty *input = c->input();
    if(input && input != _rootIndex)
      {
      ScPropertyItem *driven = findProperty(c, false);
      ScPropertyItem *driver = findProperty(input, true);

      if(driven && driver)
        {
        addConnector(c, input, driven, driver);
        }
      }

    SPropertyContainer *cont = c->castTo<SPropertyContainer>();
    if(cont)
      {
      addConnectors(cont);
      }
    }
  }

void ScNodeDisplay::addConnector(SProperty *dvnProp, SProperty *dvrProp, ScPropertyItem *driven, ScPropertyItem *driver)
  {
  xAssert(driven);
  xAssert(driver);
  xAssert(_connector);
  QObject *created = _connector->beginCreate(_connector->creationContext());
  if(!created)
    {
    qWarning() << _connector->errorString();
    return;
    }

  ScConnectorItem* connector = qobject_cast<ScConnectorItem*>(created);
  xAssert(connector);
  if(!connector)
    {
    delete created;
    qWarning() << "node property of NodeDisplay must contain a NodeItem at highest level";
    return;
    }

  _connectors << connector;
  connector->setParentItem(this);
  connector->setProperties(dvrProp, dvnProp);
  driver->addDriver(connector);
  driven->addDriven(connector);
  connector->setNodeDisplay(this);

  _connector->completeCreate();
  }
