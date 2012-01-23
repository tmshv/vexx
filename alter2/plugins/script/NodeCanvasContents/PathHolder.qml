// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
  id: holder
  property alias model: repeater.model
  property variant nameData: [ ]

  smooth: true
  border.color: "#555555"
  border.width: 1
  radius: 4
  gradient: Gradient {
    GradientStop { position: 0.0; color: "#333333" }
    GradientStop { position: 0.6; color: "#333333" }
    GradientStop { position: 1.0; color: "#444444" }
  }

  width: row.width + 10
  height: row.height + 10

  Row {
    id: row
    spacing: 3
    anchors.centerIn: parent

    Repeater {
      id: repeater
      PathItem {
        text: {
          if(modelData === 0)
          {
            return "ROOTICON";
          }

          return nameData[modelData];
        }

        onClicked: {
          var up = repeater.count - modelData - 1;
          if(up > 0)
            {
            nodecanvas.setRootToParent(up);
            }
        }
      }
    }
  }
}
