// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
  id: hoverRect
  property bool hovered: false

  opacity: 0.0
  color: "white"

  states: [
    State {
        name: "NoHover"
        when: !hovered
        PropertyChanges { target: hoverRect; opacity: 0.0 }
    },
    State {
        name: "Hovered"
        when: hovered
        PropertyChanges { target: hoverRect; opacity: 0.2 }
    }
  ]

  transitions: [
    Transition {
      from: "*"; to: "*"

      SequentialAnimation {
        NumberAnimation { property: "opacity"; easing.type: Easing.InQuart; duration: 250 }
      }
    }
  ]
}
