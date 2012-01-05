// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import VexxQMLExtensions 1.0

Ellipse {
  id: ellipse
  property alias size: ellipse.width

  signal startDrag(real x, real y)
  signal moveDrag(real x, real y)
  signal endDrag(real x, real y)

  border.color: Qt.darker(color, 1.4)
  border.width: 1
  height: width

  MouseArea {
    anchors.fill: parent
    preventStealing: true

    onPressed: {
      var gc = mapToItem(nodecanvas, size/2, size/2)
      ellipse.startDrag(gc.x, gc.y);
    }

    onMousePositionChanged: {
      var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
      ellipse.moveDrag(gc.x, gc.y);
    }

    onReleased: {
      var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
      ellipse.endDrag(gc.x, gc.y);
    }
  }
}
