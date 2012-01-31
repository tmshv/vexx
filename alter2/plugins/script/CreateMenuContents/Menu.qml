import QtQuick 1.1

Rectangle {
  property string title: ""
  property real titleHeight: title !== "" ? 15 : 0
  property real padding: 5
  property variant subMenuItem: null

  id: contents
  width: 100
  height: list.height + padding*2.0 + titleHeight
  color: "#222222"
  border.width: 1
  border.color: "#555555"
  radius: 3
  smooth: true

  state: "Hidden"
  states: [
    State {
      name: "Hidden"

      PropertyChanges {
        target: contents
        opacity: 0.0
        }
      PropertyChanges {
        target: list
        height: 0.0
        }
      },

    State {
      name: "Shown"

      PropertyChanges {
        target: contents
        opacity: 1.0
        }
      PropertyChanges {
        target: list
        height: list.childrenRect.height
        }
      },

    State {
      name: "Done"

      PropertyChanges {
        target: contents
        opacity: 0.0
        }
      }
    ]

  transitions: [
    Transition {
      from: "*"; to: "Done"
      SequentialAnimation {
        ParallelAnimation {
          PropertyAnimation { property: "opacity"; easing.type: Easing.OutCubic; duration: 700 }
          PropertyAnimation { property: "height"; easing.type: Easing.OutBounce; duration: 1500 }
          }

        ScriptAction {
          script: {
            print("Going doen");
            contents.destroy();
            }
          }
        }
      },
    Transition {
      from: "*"; to: "*"
      PropertyAnimation { property: "opacity"; easing.type: Easing.OutCubic; duration: 700 }
      PropertyAnimation { property: "height"; easing.type: Easing.OutBounce; duration: 1500 }
      }
    ]

  Text {
    text: title
    color: "white"
    font.bold: true
    x: 5
    y: (padding + titleHeight)/2.0 - height/2.0
  }

  Component.onCompleted: {
    external.focusLost.connect(focusLost);
    grabber.start();
  }

  Column {
    id: list
    y: padding + titleHeight

    width: parent.width
    height: childrenRect.height

    Repeater {
      model: ListModel {
        ListElement {
          name: "Bill Smith"
          description: "555 3264"
          hasSubMenu: true
          }
        ListElement {
          name: "John Brown"
          description: "555 8426"
          hasSubMenu: false
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          hasSubMenu: false
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          hasSubMenu: true
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          hasSubMenu: false
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          hasSubMenu: true
          }
        }

      delegate: Rectangle {
        id: item
        border.width: 1
        border.color: "#333333"
        width: parent.width
        height: column.height

        states: [
          State {
            when: mouse.pressed
            PropertyChanges {
              target: item
              color: "#373737"
            }
          },
          State {
            when: !mouse.containsMouse
            PropertyChanges {
              target: item
              color: "#444444"
            }
          },
          State {
            when: mouse.containsMouse
            PropertyChanges {
              target: item
              color: "#555555"
            }
          }
        ]

        Timer {
          id: subMenu
          interval: 500
          onTriggered: {
            if(subMenuItem)
              {
              subMenuItem.state = "Done";
              subMenuItem = null;
              }

            var component = Qt.createComponent("Menu.qml");
            var object = component.createObject(item);

            object.title = "";
            object.x = (function(){ return item.x + item.width + 1; });
            object.y = -padding;
            object.z = 2;
            object.state = "Shown";


            subMenuItem = object;
          }
        }

        MouseArea
          {
          id: mouse
          anchors.fill: parent
          hoverEnabled: true
          onEntered: {
            if(hasSubMenu)
              {
              subMenu.start();
              }
            }
          onExited: subMenu.stop();
          }

        Column {
          id: column
          width: item.width
          height: childrenRect.height

          Text {
            x: 2
            text: name
            font.bold: true
            color: "white"
            }
          Text {
            x: 2
            text: description
            font.pixelSize: 9
            color: "white"
            }
          }

        Text {
          visible: hasSubMenu
          text: ">"
          font.bold: true
          color: "white"

          anchors.margins: 2.0
          anchors.verticalCenter: column.verticalCenter
          anchors.right: column.right
          }
        }
      }
    }
  }
