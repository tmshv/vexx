import QtQuick 1.1
import QtDesktop 0.1

Column {
  id: section
  property alias host: hostField.text
  property alias port: portField.text
  property alias enabled: hostField.enabled

  height: childrenRect.height

  Grid {
    columns: 2
    spacing: 2

    width: parent.width

    Text {
      text: "Host Address"
      horizontalAlignment: Text.AlignRight
      width: parent.width * 0.25
    }

    TextField {
      id: hostField
      height: 20
      width: parent.width * 0.75
    }

    Text {
      text: "Port"
      horizontalAlignment: Text.AlignRight
      width: parent.width * 0.25
    }

    TextField {
      id: portField
      enabled: hostField.enabled
      height: 20
      width: parent.width * 0.75
    }
  }
}
