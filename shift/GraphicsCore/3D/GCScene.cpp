#include "GCScene.h"
#include "XShader.h"
#include "XRenderer.h"
#include "XMatrix4x4"
#include "siterator.h"
#include "sprocessmanager.h"

S_IMPLEMENT_PROPERTY(GCScene)

void GCScene::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    info->add(&GCScene::activeCamera, "activeCamera");
    info->add(&GCScene::cameraTransform, "cameraTransform");
    info->add(&GCScene::cameraProjection, "cameraProjection");
    }

  if(data.registerInterfaces)
    {
    XInterface<GCScene>* ifc = info->apiInterface<GCScene>();
    ifc->addMethod<void(GCViewableTransform*), &GCScene::setCamera>("setCamera");
    }
  }

GCScene::GCScene() : XCameraCanvasController(0)
  {
  }

XCameraCanvasController::CameraInterface *GCScene::camera()
  {
  return activeCamera.pointed();
  }

void GCScene::render(XRenderer *r) const
  {
  r->setProjectionTransform(cameraProjection());

  r->pushTransform(cameraTransform());

  GCRenderArray::render(r);

  r->popTransform();
  }

void GCScene::setCamera(GCViewableTransform *e)
  {
  cameraProjection.setInput(&e->projection);
  cameraTransform.setInput(&e->viewTransform);
  activeCamera.setInput(e);
  }

S_IMPLEMENT_PROPERTY(GCManipulatableScene)

void computeManips(const SPropertyInstanceInformation *, GCManipulatableScene *s)
  {
  s->refreshManipulators();
  }

void GCManipulatableScene::createTypeInformation(SPropertyInformation *info, const SPropertyInformationCreateData &data)
  {
  if(data.registerAttributes)
    {
    SPropertyArray::InstanceInformation *manInfo = info->add(&GCManipulatableScene::manipulators, "manipulators");
    manInfo->setCompute(computeManips);

    PointerArray::InstanceInformation *selInfo = info->add(&GCManipulatableScene::selection, "selection");
    selInfo->setAffects(manInfo);
    }
  }

GCManipulatableScene::GCManipulatableScene() : _currentManipulator(0), _mouseSelecting(false)
  {
  }

void GCManipulatableScene::clearManipulators()
  {
  manipulators.clear();
  }

void GCManipulatableScene::refreshManipulators()
  {
  SBlock b(handler());

  clearManipulators();

  for(GCRenderablePointer* groupPtr = renderGroup.firstChild<GCRenderablePointer>(); groupPtr; groupPtr = groupPtr->nextSibling<GCRenderablePointer>())
    {
    SProperty* inp = groupPtr->input();

    if(inp)
      {
      GCRenderable* group = inp->uncheckedCastTo<GCRenderable>(); // pointer() only returns const...
      if(group)
        {
        //group->addManipulators(&manipulators);
        }
      }
    }
  }

void GCManipulatableScene::render(XRenderer *x) const
  {
  xAssert(SProcessManager::isMainThread());

  GCScene::render(x);

  const GCCamera *cam = activeCamera();
  xAssert(cam);
  if(cam && manipulators.firstChild())
    {
    x->clear(XRenderer::ClearDepth);

    x->pushTransform(cameraTransform());
    for(GCVisualManipulator* m = manipulators.firstChild<GCVisualManipulator>(); m; m = m->nextSibling<GCVisualManipulator>())
      {
      m->render(cam, x);
      }
    x->popTransform();
    }
  }

GCManipulatableScene::UsedFlags GCManipulatableScene::mouseEvent(const MouseEvent &e)
  {
  UsedFlags parentFlags = XCameraCanvasController::mouseEvent(e).hasFlag(Used);
  if(parentFlags.hasFlag(Used))
    {
    return parentFlags;
    }

  GCVisualManipulator::MouseMoveEvent manipEv;
  manipEv.cam = activeCamera();

  manipEv.widgetPoint = e.point;

  manipEv.direction = manipEv.cam->worldSpaceFromScreenSpace(e.point.x(), e.point.y());
  manipEv.direction -= manipEv.cam->transform().translation();
  manipEv.direction.normalize();

  if(!isMouseSelecting())
    {
    if(!_currentManipulator && e.type == XAbstractCanvasController::Press && e.modifiers == Qt::NoModifier && e.triggerButton == Qt::LeftButton)
      {
      float chosenDistance = HUGE_VAL;
      GCVisualManipulator *chosenManip = 0;

      for(GCVisualManipulator* m = manipulators.firstChild<GCVisualManipulator>(); m; m = m->nextSibling<GCVisualManipulator>())
        {
        float tempDistance = HUGE_VAL;
        GCVisualManipulator *clicked = 0;
        if(m->hitTest(e.point, manipEv.cam, manipEv.direction, &tempDistance, &clicked))
          {
          if(tempDistance < chosenDistance)
            {
            chosenManip = clicked;
            chosenDistance = tempDistance;
            xAssert(chosenManip);
            }
          }
        }
      _currentManipulator = chosenManip;
      }

    if(_currentManipulator)
      {
      if(e.type == XAbstractCanvasController::Press)
        {
        _currentManipulator->onMouseClick(manipEv);
        }
      else if(e.type == XAbstractCanvasController::Release)
        {
        _currentManipulator->onMouseRelease(manipEv);
        _currentManipulator = 0;
        }
      else if(e.type == XAbstractCanvasController::DoubleClick)
        {
        _currentManipulator->onMouseDoubleClick(manipEv);
        // reset current manip?
        }
      else if(e.type == XAbstractCanvasController::Move)
        {
        manipEv.lastDirection = manipEv.cam->worldSpaceFromScreenSpace(e.lastPoint.x(), e.lastPoint.y());
        manipEv.lastDirection -= manipEv.cam->transform().translation();
        manipEv.lastDirection.normalize();

        manipEv.lastWidgetPoint = e.lastPoint;

        _currentManipulator->onMouseDrag(manipEv);
        }

      return Used|NeedsUpdate;
      }
    }
  else
    {
    if(e.type == Move)
      {
      moveMouseSelection(manipEv.direction);
      }
    if(e.type == Release)
      {
      endMouseSelection(manipEv.direction);
      }
    }

  if(!_currentManipulator && e.type == Press && e.modifiers == Qt::NoModifier && e.triggerButton == Qt::LeftButton)
    {
    beginMouseSelection(manipEv.direction);
    }

  return NotUsed;
  }

GCManipulatableScene::UsedFlags GCManipulatableScene::wheelEvent(const WheelEvent &w)
  {
  return XCameraCanvasController::wheelEvent(w);
  }


void GCManipulatableScene::beginMouseSelection(const XVector3D &sel)
  {
  _mouseSelecting = true;
  _hasMouseMoved = false;
  _initialRay = sel;
  }

void GCManipulatableScene::moveMouseSelection(const XVector3D &sel)
  {
  _hasMouseMoved = true;
  _finalRay = sel;
  }

void GCManipulatableScene::endMouseSelection(const XVector3D &sel)
  {
  _mouseSelecting = false;
  _finalRay = sel;

  if(_hasMouseMoved)
    {
    //marqueeSelect();
    raySelect(_initialRay);
    }
  else
    {
    raySelect(_initialRay);
    }
  }

bool GCManipulatableScene::isMouseSelecting() const
  {
  return _mouseSelecting;
  }

void GCManipulatableScene::raySelect(const XVector3D &)
  {
  /*GCShadingGroupPointer* groupPtr = shadingGroups.firstChild<GCShadingGroupPointer>();
  GCShadingGroup* g = groupPtr->input()->uncheckedCastTo<GCShadingGroup>();
  GCGeometryTransform *t = g->geometry.firstChild()->input()->castTo<GCGeometryTransform>();*/

  // todo, fill this in.
  xAssertFail();
  }

void GCManipulatableScene::marqueeSelect(const XFrustum &)
  {
  xAssertFail();
  }
