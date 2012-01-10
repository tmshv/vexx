// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


Rectangle {
    id: header

    property bool dragging: false
    property alias text: headerText.text

    signal createConnection(variant item, string mode, real x, real y)
    signal dragged(real x, real y)

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

    MouseArea {
        property real lastX: 0
        property real lastY: 0

        anchors.fill: parent
        preventStealing: true

        onPressed: {
            nodecanvas.bringToTop(nodeItem);
            dragging = true
            var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
            lastX = gc.x
            lastY = gc.y
        }

        onDoubleClicked: nodecanvas.setRootToChildIndex(index)
        onReleased: dragging = false
        onMousePositionChanged: {
            var gc = mapToItem(nodecanvas, mouse.x, mouse.y)

            header.dragged(gc.x - lastX, gc.y - lastY)

            lastX = gc.x
            lastY = gc.y
        }
    }
}
