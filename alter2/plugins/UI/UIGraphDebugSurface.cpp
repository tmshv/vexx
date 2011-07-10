#include "UIGraphDebugSurface.h"
#include "UIPlugin.h"
#include "QPainter"
#include "QMenu"
#include "styperegistry.h"

UIGraphDebugSurface::UIGraphDebugSurface(SEntity *ent) : UISurface("Graph Debug", _canvas = new X2DCanvas(), UISurface::Dock),
    GCShiftRenderModel(ent, SIterator::createFilter<DirectEntityChildren>()),
    _iterator(0), _controller(_canvas)
  {
  _canvas->setBackgroundColour(QColor(128, 128, 128));
  _canvas->setModel(this);
  _canvas->setAntiAliasingEnabled(true);
  _canvas->setController(&_controller);

  connect(&_controller, SIGNAL(onContextMenu(QPoint)), this, SLOT(onContextMenu(QPoint)));
  }

UIGraphDebugSurface::~UIGraphDebugSurface()
  {
  if(_iterator)
    {
    delete _iterator;
    }
  }

const XAbstractDelegate *UIGraphDebugSurface::delegateFor(XAbstractRenderModel::Iterator *, const XAbstractCanvas *c) const
  {
  xAssert(c == _canvas);
  return &_delegate;
  }

void UIGraphDebugSurface::onContextMenu(QPoint point)
  {
  QMenu menu;

  SEntity *ent = 0;
  GCAbstractNodeDelegate::HitArea area = GCAbstractNodeDelegate::None;
  xsize index = X_SIZE_SENTINEL;
  if(!_iterator)
    {
    _iterator = createIterator();
    }

  resetIterator(_iterator);
  while(_iterator->next())
    {
    Iterator *i = static_cast<Iterator*>(_iterator);
    xsize idx = X_SIZE_SENTINEL;
    GCAbstractNodeDelegate::HitArea hA = _delegate.hitTest(point, i->entity(), idx);
    if(hA != GCAbstractNodeDelegate::None)
      {
      area = hA;
      ent = i->entity();
      index = idx;
      }
    }


  if(ent == 0)
    {
    menu.addAction("Create Entity")->setEnabled(false);
    generatePropertyActions(SEntity::staticTypeInformation(), &menu);

    connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(createEntity(QAction*)));
    }
  else
    {
    if(area == GCAbstractNodeDelegate::Input)
      {
      SProperty *prop = ent->at(index);

      if(prop->hasInput())
        {
        QAction *act = menu.addAction("Disconnect Input");
        act->setData((quint64)prop);
        connect(act, SIGNAL(triggered()), this, SLOT(disconnect()));
        }
      }
    else if(area == GCAbstractNodeDelegate::Output)
      {
      SProperty *prop = ent->at(index);

      if(prop->hasOutputs())
        {
        QAction *act = menu.addAction("Disconnect Output");
        act->setData((quint64)prop);
        connect(act, SIGNAL(triggered()), this, SLOT(disconnect()));
        }
      }
    else if(area == GCAbstractNodeDelegate::NodeDull || area == GCAbstractNodeDelegate::NodeTranslatable)
      {
      QAction *act = menu.addAction("Delete Entity");
      act->setData((quint64)ent);
      connect(act, SIGNAL(triggered()), this, SLOT(deleteEntity()));
      }
    }

  if(menu.actions().size())
    {
    menu.exec(_canvas->mapToGlobal(point));
    }
  }

void UIGraphDebugSurface::generatePropertyActions(const SPropertyInformation *mustInheritFrom, QMenu *menuToAdd)
  {
  if(entity())
    {
    SDatabase *db = entity()->database();

    if(db)
      {
      QMap<QChar, QMap<QString, const SPropertyInformation *> > data;

      foreach(const SPropertyInformation *type, STypeRegistry::types())
        {
        if(type->inheritsFromType(mustInheritFrom))
          {
          QChar key = type->typeName().toUpper()[0];
          data[key][type->typeName()] = type;
          }
        }

      foreach(QChar letter, data.keys())
        {
        const QMap<QString, const SPropertyInformation *> &contents = data.value(letter);
        QMenu *menu = menuToAdd->addMenu(letter);

        foreach(QString typeName, contents.keys())
          {
          menu->addAction(typeName);
          }
        }
      }
    }
  }

void UIGraphDebugSurface::disconnect()
  {
  QAction *act = qobject_cast<QAction*>(sender());
  if(act)
    {
    SProperty *prop = (SProperty *)act->data().toULongLong();
    if(prop)
      {
      prop->disconnect();
      }
    }
  }

void UIGraphDebugSurface::deleteEntity()
  {
  QAction *act = qobject_cast<QAction*>(sender());
  if(act)
    {
    SEntity *ent = (SEntity *)act->data().toULongLong();
    if(ent)
      {
      ent->parentEntity()->children.remove(ent);
      }
    }
  }

void UIGraphDebugSurface::createEntity(QAction *act)
  {
  if(entity())
    {
    const SPropertyInformation *type = STypeRegistry::findType(act->text());

    if(type)
      {
      entity()->addChild(type, "NewEntity");
      }
    }
  }

void UIGraphDebugSurface::createProperty(QAction*)
  {
  }
