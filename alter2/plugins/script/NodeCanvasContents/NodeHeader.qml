import QtQuick 1.1
import VexxQMLExtensions 1.0

Rectangle {
    id: header

    property alias showInterfaces: nodeInputBlob.visible
    property bool dragging: false
    property alias text: headerText.text

    signal bringToTop()
    signal enter()
    signal createConnection(variant item, string mode, real x, real y)
    signal dragged(real x, real y)

    function drivenPoint()
      {
      return Qt.point(header.x + nodeInputBlob.x + nodeInputBlob.size/2-2,
                      header.y + nodeInputBlob.y + nodeInputBlob.size/2+2);
      }

    function driverPoint()
      {
      return Qt.point(header.x + nodeOutputBlob.x + nodeOutputBlob.size/2,
                      header.y + nodeOutputBlob.y + nodeOutputBlob.size/2+2);
      }

    function intersect(x, y)
      {
      var isect = null;
      if(!isect && nodecanvas.intersectItem(nodeOutputBlob, x, y))
        {
        isect = nodeOutputBlob;
        }

      if(!isect && nodecanvas.intersectItem(nodeInputBlob, x, y))
        {
        isect = nodeInputBlob;
        }

      if(!isect && nodecanvas.intersectItem(header, x, y))
        {
        isect = header;
        }
      return isect;
      }

    smooth: true
    height: 20
    border.color: Qt.darker(color, 1.4)
    border.width: 2
    radius: 3

    PropertyInterface {
      id: nodeInputBlob
      x: -nodeInputBlob.size/2 - 1
      y: (header.height/2) + 1 - nodeInputBlob.size/2
      size: 15
      color: header.color

      onStartDrag: {
        createConnection(nodeInputBlob, "input", x, y);
      }
    }

    PropertyInterface {
      id: nodeOutputBlob
      x: nodeInputBlob.x + header.width + 3
      y: nodeInputBlob.y
      size: nodeInputBlob.size
      color: header.color
      visible: nodeInputBlob.visible

      onStartDrag: {
        createConnection(nodeOutputBlob, "output", x, y);
      }
    }


    Text {
        id: headerText
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.bold: true
        font.pointSize: 10
        elide: Text.ElideRight
        color: Qt.darker(header.color, 2.0)
    }

    MouseAreaV2 {
        property real lastX: 0
        property real lastY: 0

        anchors.fill: parent
        preventStealing: true

        onPressed: {
          bringToTop();
          dragging = true
          var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
          lastX = gc.x
          lastY = gc.y
        }

        onDoubleClicked: enter()
        onReleased: dragging = false
        onMousePositionChanged: {
            var gc = mapToItem(nodecanvas, mouse.x, mouse.y)

            header.dragged(gc.x - lastX, gc.y - lastY)

            lastX = gc.x
            lastY = gc.y
        }
    }
}
