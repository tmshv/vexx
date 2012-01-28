// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import VexxQMLExtensions 1.0

Ellipse {
  id: ellipse
  property alias size: ellipse.width

  signal startDrag(real x, real y)
  signal moveDrag(real x, real y)
  signal endDrag(real x, real y)

  function attachPoint(rel)
    {
    var pt = ellipse.mapToItem(rel, size/2, size/2+2);
    return Qt.point(pt.x, pt.y);
    }

  border.color: Qt.darker(color, 1.4)
  border.width: 1
  height: width

  MouseArea {
    property bool dragging: false
    anchors.fill: parent
    preventStealing: true

    onPressed: {
      mouse.accepted = false;
      if(mouse.modifiers === 0)
        {
        dragging = true;
        var gc = mapToItem(nodecanvas, size/2, size/2)
        ellipse.startDrag(gc.x, gc.y);
        mouse.accepted = true;
        }
      }

    onMousePositionChanged: {
      mouse.accepted = false;
      if(dragging)
        {
        var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
        ellipse.moveDrag(gc.x, gc.y);
        mouse.accepted = true;
        }
      }

    onReleased: {
      mouse.accepted = false;
      if(dragging)
        {
        dragging = false;
        var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
        ellipse.endDrag(gc.x, gc.y);
        mouse.accepted = true;
        }
      }
    }
  }
