#include "GCScene.h"
#include "XShader.h"
#include "XRenderer.h"
#include "XMatrix4x4"
#include "siterator.h"

S_IMPLEMENT_PROPERTY(GCScene)

SPropertyInformation *GCScene::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCScene>("GCScene");

  info->add(&GCScene::activeCamera, "activeCamera");
  info->add(&GCScene::cameraTransform, "cameraTransform");
  info->add(&GCScene::cameraProjection, "cameraProjection");

  info->add(&GCScene::shadingGroups, "shadingGroups");

  return info;
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

  for(const GCShadingGroupPointer* groupPtr = shadingGroups.firstChild<GCShadingGroupPointer>(); groupPtr; groupPtr = groupPtr->nextSibling<GCShadingGroupPointer>())
    {
    const GCShadingGroup* group = groupPtr->pointed();
    if(group)
      {
      group->render(r);
      }
    }

  r->popTransform();
  }

S_IMPLEMENT_PROPERTY(GCManipulatableScene)

void computeManips(const SPropertyInstanceInformation *inst, SPropertyContainer *cont)
  {
  GCManipulatableScene *s = cont->uncheckedCastTo<GCManipulatableScene>();
  s->refreshManipulators();
  }

SPropertyInformation *GCManipulatableScene::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCManipulatableScene>("GCManipulatableScene");

  SPropertyArray::InstanceInformation *manInfo = info->add(&GCManipulatableScene::manipulators, "manipulators");
  manInfo->setCompute(computeManips);

  PointerArray::InstanceInformation *selInfo = info->add(&GCManipulatableScene::selection, "selection");
  selInfo->setAffects(manInfo);

  return info;
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
  SBlock b(database());

  clearManipulators();

  for(GCShadingGroupPointer* groupPtr = shadingGroups.firstChild<GCShadingGroupPointer>(); groupPtr; groupPtr = groupPtr->nextSibling<GCShadingGroupPointer>())
    {
    GCShadingGroup* group = groupPtr->input()->uncheckedCastTo<GCShadingGroup>(); // pointer() only returns const...
    if(group)
      {
      group->addManipulators(&manipulators);
      }
    }
  }

void GCManipulatableScene::render(XRenderer *x) const
  {
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

void GCManipulatableScene::raySelect(const XVector3D &sel)
  {
  GCShadingGroup *g = shadingGroups.firstChild<GCShadingGroup>();
  GCGeometryTransform *t = g->geometry.firstChild()->input()->castTo<GCGeometryTransform>();

  }

void GCManipulatableScene::marqueeSelect(const XFrustum &frus)
  {
  xAssertFail();
  }
