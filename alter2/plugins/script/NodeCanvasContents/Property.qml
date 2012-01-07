import QtQuick 1.1

Item {
  id: propertyContainer

  property alias text: label.text
  property alias colour: inputBlob.color
  property real contentsOffset: 0
  property string mode: ""

  property variant input: null

  signal propertyChanged(variant prop)

  function setupProperty()
    {
    propertyContainer.propertyChanged(propertyContainer);
    expand.setupExpand();

    nodecanvas.setupInput(propertyContainer, getModelIndex());
    }

  function getModelIndex()
    {
    return propertyList.childIndex(index)
    }

  function childItem(index)
    {
    var children = childListHolder.children[0];
    if(children)
      {
      return children.childItem(index);
      }

    print("Asking for child from non children node.");
    return null;
    }

  function getInputPosition(relative)
    {
    var mapped = inputBlob.mapToItem(relative, inputBlob.width/2, inputBlob.height/2+2);
    return Qt.point(mapped.x, mapped.y);
    }

  function getOutputPosition(relative)
    {
    var mapped = outputBlob.mapToItem(relative, outputBlob.width/2, outputBlob.height/2+1);
    return Qt.point(mapped.x, mapped.y);
    }

  width: col.width
  height: col.height
  visible: mode != "internal"

  state: "NotExpandable"

  states: [
    State {
        name: "NotExpandable"
        PropertyChanges { target: childListHolder; visible: false }
        PropertyChanges { target: expand; visible: false }
    },
    State {
        name: "Expandable"
        PropertyChanges { target: childListHolder; visible: false; opacity: 0.0; height: 0 }
        PropertyChanges { target: expand; rotation: 270 }
    },
    State {
        name: "Expanded"
        PropertyChanges { target: childListHolder; visible: true; opacity: 1.0; height: childListHolder.childrenRect.height }
        PropertyChanges { target: expand; rotation: 0 }
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
    visible: mode != "computed" && mode != "output"

    onStartDrag: nodecanvas.startCreatingConnection(inputBlob, "input", x, y)
  }

  PropertyInterface {
    id: outputBlob
    anchors.top: inputBlob.top
    x: propertyContainer.width + 1 - inputBlob.size/2
    z: 2.0
    color: inputBlob.color
    size: inputBlob.size
    visible: mode != "input"

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

        MouseArea {
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
                object.xOffset = (function() { return propertyContainer.contentsOffset + nodeItem.propertyTabIn; });
                object.rootIndex = (function() { return propertyList.childIndex(index); });
              }
              propertyContainer.state = "Expanded";
            }
            else
            {
              propertyContainer.state = "Expandable";
            }
          }

        }

      function setupExpand()
        {
        var myModelIndex = propertyList.childIndex(index);
        var hasChildren = db.rowCount(myModelIndex) !== 0;

        if(hasChildren)
          {
          propertyContainer.state = "Expandable";
          }
        else
          {
          propertyContainer.state = "NotExpandable";
          }
        }
      }

      Text {
        id: label
        // hack, the component seems to be reused on property adds without calling Component.onCompleted()... so this catches it.
        onTextChanged: propertyContainer.setupProperty()
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
