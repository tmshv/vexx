// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

MouseArea {
  property bool dragging: false
  property real attachPointX: width/2
  property real attachPointY: height/2
  preventStealing: true


  signal startDrag(real x, real y)
  signal moveDrag(real x, real y)
  signal endDrag(real x, real y)

  onPressed: {
    mouse.accepted = false;
    if(mouse.modifiers === 0)
      {
      dragging = true;

      var gc = mapToItem(nodecanvas, attachPointX, attachPointY)
      startDrag(gc.x, gc.y);
      mouse.accepted = true;
      }
    }

  onMousePositionChanged: {
    mouse.accepted = false;
    if(dragging)
      {
      var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
      moveDrag(gc.x, gc.y);
      mouse.accepted = true;
      }
    }

  onReleased: {
    mouse.accepted = false;
    if(dragging)
      {
      dragging = false;
      var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
      endDrag(gc.x, gc.y);
      mouse.accepted = true;
      }
    }
  }
