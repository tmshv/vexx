#include "GCNodeController.h"
#include "XAbstractCanvas.h"
#include "GCShiftRenderModel.h"
#include "GCAbstractNodeDelegate.h"
#include "sentity.h"
#include "X2DCanvas.h"

GCNodeController::GCNodeController(X2DCanvas *c) : XSimple2DCanvasController(c), _iterator(0),
  _interactionEntity(0), _interactionProperty(X_SIZE_SENTINEL), _interactionDelegate(0),
  _connectingOutput(false), _controlMode(None)
  {
  }

GCNodeController::~GCNodeController()
  {
  if(_iterator)
    {
    xAssert(canvas());
    canvas()->model()->destroyIterator(_iterator);
    }
  }

int GCNodeController::mouseEvent(MouseEventType type,
                                     QPoint point,
                                     Qt::MouseButton triggerButton,
                                     Qt::MouseButtons buttonsDown,
                                     Qt::KeyboardModifiers modifiers)
  {

  QPoint mouseDelta(point - lastKnownMousePosition());

  int result = XSimple2DCanvasController::mouseEvent(type, point, triggerButton, buttonsDown, modifiers);
  if(result != NotUsed)
    {
    return result;
    }

  if(!_iterator)
    {
    _iterator = canvas()->model()->createIterator();
    }

  if(type == Move && _controlMode == MovingEntity && _interactionEntity)
    {
    xAssert(_interactionDelegate);
    xAssert(_interactionEntity);
    _interactionDelegate->move(mouseDelta, _interactionEntity);

    result |= Used|NeedsUpdate;
    }
  if(type == Move && _controlMode == ConnectingProperty && _interactionEntity)
    {
    result = Used|NeedsUpdate;
    }
  if(triggerButton == Qt::LeftButton)
    {
    if(type == Press)
      {
      xAssert(_interactionEntity == 0);
      xAssert(_controlMode == None);
      canvas()->model()->resetIterator(_iterator);
      while(_iterator->next())
        {
        const GCAbstractNodeDelegate *delegate = static_cast<const GCAbstractNodeDelegate*>(canvas()->model()->delegateFor(_iterator, canvas()));

        const GCShiftRenderModel::Iterator *it = static_cast<const GCShiftRenderModel::Iterator *>(_iterator);
        xsize index = X_SIZE_SENTINEL;
        GCAbstractNodeDelegate::HitArea hitResult = delegate->hitTest(point, it->entity(), index);

        if(hitResult != GCAbstractNodeDelegate::None)
          {
          _interactionEntity = 0;
          _controlMode = None;
          _interactionDelegate = delegate;

          if(hitResult == GCAbstractNodeDelegate::NodeTranslatable)
            {
            _interactionEntity = it->entity();
            _controlMode = MovingEntity;
            }
          else if(hitResult == GCAbstractNodeDelegate::Input)
            {
            _controlMode = ConnectingProperty;
            _connectingOutput = false;
            _interactionEntity = it->entity();
            _interactionProperty = index;
            }
          else if(hitResult == GCAbstractNodeDelegate::Output)
            {
            _controlMode = ConnectingProperty;
            _connectingOutput = true;
            _interactionEntity = it->entity();
            _interactionProperty = index;
            }
          }
        }

      result |= Used;
      }
    else if(type == Release && _controlMode != None)
      {
      if(_controlMode == ConnectingProperty)
        {
        canvas()->model()->resetIterator(_iterator);
        while(_iterator->next())
          {
          const GCAbstractNodeDelegate *delegate = static_cast<const GCAbstractNodeDelegate*>(canvas()->model()->delegateFor(_iterator, canvas()));

          const GCShiftRenderModel::Iterator *it = static_cast<const GCShiftRenderModel::Iterator*>(_iterator);
          xsize index = X_SIZE_SENTINEL;
          GCAbstractNodeDelegate::HitArea hitResult = delegate->hitTest(point, it->entity(), index);

          SProperty *a = _interactionEntity->at(_interactionProperty);
          if(_connectingOutput && hitResult == GCAbstractNodeDelegate::Input)
            {
            SProperty *b = it->entity()->at(index);
            a->connect(b);
            }
          else if(!_connectingOutput && hitResult == GCAbstractNodeDelegate::Output)
            {
            SProperty *b = it->entity()->at(index);
            b->connect(a);
            }
          }

        }
      _controlMode = None;
      _interactionEntity = 0;
      _interactionDelegate = 0;

      result |= Used|NeedsUpdate;
      }
    }
  else if(type == Press && triggerButton == Qt::RightButton)
    {
    emit onContextMenu(point);
    }

  lastKnownMousePosition() = point;
  return result;
  }

xuint32 GCNodeController::maxNumberOfPasses(xuint32 modelMaxPasses) const
  {
  _connectionPass = modelMaxPasses;
  return modelMaxPasses + 1;
  }

void GCNodeController::paint(xuint32 pass) const
  {
  if(pass == _connectionPass && _controlMode == ConnectingProperty)
    {
    xAssert(_interactionEntity);

    _interactionDelegate->drawConnection(canvas(), _interactionEntity, _interactionProperty, _connectingOutput, lastKnownMousePosition());
    }
  }
