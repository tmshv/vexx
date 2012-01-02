import QtQuick 1.1

Rectangle {
  id: propertyContainer

  property alias text: label.text
  property alias colour: inputBlob.color

  signal propertyChanged(variant prop)

  function setupProperty()
    {
    propertyContainer.propertyChanged(propertyContainer);
    expand.setupExpand();

    nodecanvas.setupInput(propertyContainer, propertyList.childIndex(index));
    }

  function getChildItem(index)
    {
    var children = childListHolder.children[1];
    if(children)
      {
      return children.getChildItem(index);
      }

    return null;
    }

  function getInputPosition(relative)
    {
    var mapped = propertyContainer.mapFromItem(relative, 0, 0);
    return mapped;
    }

  function getOutputPosition(relative)
    {
    var mapped = propertyContainer.mapToItem(relative, 0, 0);
    mapped.x += propertyContainer.width;
    return mapped;
    }

  color: "transparent"
  width: col.width
  height: col.height

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


  Column {
    id: col
    Row {
      id: grouper
      spacing: 5

      PropertyInterface {
        id: inputBlob
        y: 2
      }

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

                object.rootIndex = (function() { return propertyList.childIndex(index); })
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

    Row {
      id: childListHolder
      visible: false

      Rectangle {
        id: spacer
        width: 10
        height: 10
        color: "transparent"
      }
    }
  }
}
