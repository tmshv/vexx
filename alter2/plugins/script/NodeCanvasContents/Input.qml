import QtQuick 1.1
import VexxQMLExtensions 1.0

ConnectorItem {
  id: inputHolder

  signal moveDrag(real x, real y)
  signal endDrag(real x, real y)

  MouseArea {
    property bool dragging: false
    anchors.fill: path

    onPressed: {
      mouse.accepted = false;
      if(mouse.modifiers === 0)
        {
        var hit = path.intersect(mouse.x, mouse.y, .0);
        if(hit > 0.0 && hit < 2.0)
          {
          mouse.accepted = true;
          var mode = "input";
          if(hit <= 0.5)
            {
            mode = "output";
            }
          nodecanvas.editConnection(inputHolder, mode, mouse.x, mouse.y);
          dragging = true;
          }
        }
      }
    onMousePositionChanged: {
      if(dragging)
        {
        inputHolder.moveDrag(mouse.x, mouse.y);
        }
      }
    onReleased: {
      if(dragging)
        {
        inputHolder.endDrag(mouse.x, mouse.y);
        dragging = false;
        }
      }
    }

  Path {
    id: path
    width: 4

    firstColour: driverColour
    lastColour: drivenColour

    firstNormal.x: driverNormal.x * 100
    firstNormal.y: driverNormal.y * 100
    lastNormal.x: drivenNormal.x * 100
    lastNormal.y: drivenNormal.y * 100

    firstPoint: {
      return inputHolder.driverPoint
    }
    lastPoint: {
      return inputHolder.drivenPoint
    }
  }
}
