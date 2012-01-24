import QtQuick 1.1
import VexxQMLExtensions 1.0

PropertyItem {
  id: propertyContainer
  property real xOffset: 0

  width: parent.width
  height: col.height

  driverPoint: outputBlob.attachPoint(propertyContainer)
  drivenPoint: inputBlob.attachPoint(propertyContainer)
  driverNormal.x: 1.0
  driverNormal.y: 0.0
  drivenNormal.x: -1.0
  drivenNormal.y: 0.0

  function intersect(x, y)
    {
    var isect = null;
    if(!isect && nodecanvas.intersectItem(outputBlob, x, y))
      {
      isect = outputBlob;
      }

    if(!isect && nodecanvas.intersectItem(inputBlob, x, y))
      {
      isect = inputBlob;
      }
    return isect;
    }

  state: "NotExpandable"

  states: [
    State {
        name: "NotExpandable"
        when: !propertyContainer.hasChildren
        PropertyChanges { target: childListHolder; visible: false }
        PropertyChanges { target: expand; visible: false }
    },
    State {
        name: "Expandable"
        when: propertyContainer.hasChildren
        PropertyChanges { target: childListHolder; visible: false; opacity: 0.0; height: 0 }
        PropertyChanges { target: expand; rotation: 270 }
        PropertyChanges { target: expand; visible: true }
    },
    State {
        name: "Expanded"
        PropertyChanges { target: childListHolder; visible: true; opacity: 1.0; height: childListHolder.childrenRect.height }
        PropertyChanges { target: expand; rotation: 0 }
        PropertyChanges { target: expand; visible: true }
    }
  ]

  transitions: [
    Transition {
      from: "Expanded"; to: "*"

      SequentialAnimation {
        NumberAnimation { property: "opacity"; easing.type: Easing.InOutQuad; duration: 400; from: 1.0; to: 0.01 }
        NumberAnimation { property: "height"; easing.type: Easing.InOutQuad; duration: 200 }
        NumberAnimation { property: "opacity"; duration: 0 }

        PropertyAnimation { properties: "visible"; duration: 0 }
      }
    },

    Transition {
      from: "Expandable"; to: "*"

      SequentialAnimation {
        PropertyAnimation { properties: "visible"; duration: 0 }

        NumberAnimation { property: "opacity"; duration: 0; from: 0.0; to: 0.01 }
        NumberAnimation { property: "height"; easing.type: Easing.InOutQuad; duration: 200 }
        NumberAnimation { property: "opacity"; easing.type: Easing.InOutQuad; duration: 400 }
      }
    }
  ]

  PropertyInterface {
    id: inputBlob
    y: 1.0
    x: 1 - inputBlob.size/2
    z: 2.0
    size: 12
    color: propertyContainer.colour
    visible: mode !== "computed" && mode !== "output"

    onStartDrag: nodecanvas.startCreatingConnection(inputBlob, "input", x, y)
  }

  PropertyInterface {
    id: outputBlob
    anchors.top: inputBlob.top
    x: propertyContainer.width + 1 - inputBlob.size/2
    z: 2.0
    color: inputBlob.color
    size: inputBlob.size
    visible: mode !== "input" && mode !== "internalinput"

    onStartDrag: nodecanvas.startCreatingConnection(outputBlob, "output", x, y)
  }

  Column {
    id: col

    Row {
      id: grouper
      spacing: 5
      x: 10 + xOffset

      Image {
        id: expand
        y: 2
        source: "qrc:/Sc/expandExpanded.svg"

        MouseAreaV2 {
          property bool setup: false
          anchors.fill: parent

          onClicked: {
            if(propertyContainer.state == "Expandable")
            {
              if(!setup)
              {
                setup = true;
                var component = Qt.createComponent("PropertyList.qml");
                var object = component.createObject(childListHolder);

                object.width = (function() { return propertyContainer.width; });
                object.xOffset = (function() { return propertyContainer.xOffset + nodeItem.propertyTabIn; });
                object.rootItem = (function() { return propertyContainer; });
              }
              propertyContainer.state = "Expanded";
            }
            else
            {
              propertyContainer.state = "Expandable";
            }
          }
        }
      }

      Text {
        id: label
        text: propertyContainer.name
        font.pointSize: 8
        color: "white"
        elide: Text.ElideRight
      }
    }

    Column {
      id: childListHolder
      visible: false
    }
  }
}
