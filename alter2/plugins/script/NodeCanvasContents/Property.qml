import QtQuick 1.1

Column {
  id: propertyContainer

  property alias text: label.text
  property alias colour: colourBlob.color
  property variant input: null

  Row {
    id: grouper

    Text {
      text: " "
    }

    Rectangle {
      id: colourBlob
      y: 2
      border.color: Qt.darker(color, 1.4)
      border.width: 1
      radius: 5
      width: 10
      height: 10

      function createInput() {
        /*if(db.isValid(input)) {
          var component = Qt.createComponent("Input.qml");
          input = component.createObject(grouper, { inputIndex: input });

          if (input == null) {
            // Error Handling
            console.log("Error creating object");
          }
        }*/
      }

      Component.onCompleted: createInput();
    }

    Text {
      text: "  "
    }

    Rectangle {
      id: expand
      y: 2
      color: childListHolder.visible ? "red" : "white"
      width: 10
      height: 10

      MouseArea {
        property bool setup: false
        anchors.fill: parent

        function toggleExpand()
          {
          if(!childListHolder.visible)
            {
            if(!setup)
              {
              setup = true;
              var component = Qt.createComponent("PropertyList.qml");
              var object = component.createObject(childListHolder);

              object.rootIndex = (function() { return propertyList.childIndex(index); })
              }
            childListHolder.visible = true;
            }
          else
            {
            childListHolder.visible = false;
            }
          }

        onClicked: toggleExpand();
      }

      function setupExpand()
        {
        var myModelIndex = propertyList.childIndex(index);
        var hasChildren = db.rowCount(myModelIndex) !== 0;

        if(hasChildren)
          {
          expand.visible = true;
          expandSpacer.visible = true;
          }
        else
          {
          expand.visible = false;
          expandSpacer.visible = false;
          childListHolder.visible = false;
          }
        }
    }

    Text {
      id: expandSpacer
      text: " "
    }

    Text {
      id: label
      onTextChanged: expand.setupExpand();
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
