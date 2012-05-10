import QtQuick 1.1
import VexxQMLExtensions 1.0

Rectangle {
  id: header

  property alias showInterfaces: nodeInputBlob.visible
  property bool dragging: false
  property alias text: headerText.text
  property alias showClose: close.visible

  signal close()
  signal click()
  signal clickReleased()
  signal rightClick()
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

    PropertyInterfaceMouseArea {
      id: inputArea
      anchors.fill: parent
      enabled: parent.visible
      onStartDrag: {
        createConnection(inputArea, "input", x, y);
      }
    }
  }

  PropertyInterface {
    id: nodeOutputBlob
    x: nodeInputBlob.x + header.width + 3
    y: nodeInputBlob.y
    size: nodeInputBlob.size
    color: header.color
    visible: nodeInputBlob.visible

    PropertyInterfaceMouseArea {
      id: outputArea
      anchors.fill: parent
      enabled: parent.visible
      onStartDrag: {
        createConnection(outputArea, "output", x, y);
      }
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

  Text {
    id: close
    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.margins: 5.0
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    font.bold: true
    font.pointSize: 10
    text: "X"
    color: Qt.darker(header.color, 2.0)
  }

  MouseArea {
    property real lastX: 0
    property real lastY: 0

    anchors.fill: parent
    preventStealing: true
    acceptedButtons: Qt.LeftButton|Qt.RightButton

    onPressed: {
      mouse.accepted = false;
      if(mouse.modifiers === 0)
        {
        mouse.accepted = true;
        if(mouse.button === Qt.LeftButton)
          {
          if(mouse.x > parent.width-15)
            {
            header.close();
            }
          else
            {
            click();
            dragging = true
            var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
            lastX = gc.x
            lastY = gc.y
            }
          }
        else if(mouse.button == Qt.RightButton)
          {
          rightClick();
          }
        }
      }

    onDoubleClicked: {
      mouse.accepted = true;
      enter()
      }

    onReleased: {
      mouse.accepted = false;
      if(dragging)
        {
        dragging = false;
        mouse.accepted = true;
        clickReleased();
        }
      }

    onMousePositionChanged: {
      mouse.accepted = mouse.modifiers === 0;
      if(dragging)
        {
        mouse.accepted = true;
        var gc = mapToItem(nodecanvas, mouse.x, mouse.y)

        var x = gc.x - lastX;
        var y = gc.y - lastY;
        if(x !== 0 || y !== 0)
          {
          header.dragged(x,   y);
          }

        lastX = gc.x
        lastY = gc.y
        }
      }
    }
  }
