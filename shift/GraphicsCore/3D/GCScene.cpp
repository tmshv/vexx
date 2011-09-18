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

SPropertyInformation *GCManipulatableScene::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCManipulatableScene>("GCManipulatableScene");

  info->add(&GCManipulatableScene::manipulators, "manipulators");

  return info;
  }


GCManipulatableScene::GCManipulatableScene() : _currentManipulator(0)
  {
  }

void GCManipulatableScene::clearManipulators()
  {
  manipulators.clear();
  }

void GCManipulatableScene::addAllManipulators()
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
  if(cam)
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
  GCVisualManipulator::MouseMoveEvent manipEv;
  manipEv.cam = activeCamera();

  manipEv.widgetPoint = e.point;

  manipEv.direction = manipEv.cam->worldSpaceFromScreenSpace(e.point.x(), e.point.y());
  manipEv.direction -= manipEv.cam->transform().translation();
  manipEv.direction.normalize();

  if(!_currentManipulator && e.type == XAbstractCanvasController::Press)
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

  return XCameraCanvasController::mouseEvent(e);
  }

GCManipulatableScene::UsedFlags GCManipulatableScene::wheelEvent(const WheelEvent &w)
  {
  return XCameraCanvasController::wheelEvent(w);
  }
