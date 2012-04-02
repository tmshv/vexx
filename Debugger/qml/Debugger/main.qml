import QtQuick 1.1
import QtDesktop 0.1
import "content"

Window {
  id: mainWindow
  title: "Debugger"

  width: 300
  height: content.height + 10
  minimumHeight: content.height + 10
  maximumHeight: content.height + 10
  visible: true

  property bool connected: false

  Column {
    id: content
    width: parent.width - 10
    y: 5
    x: 5
    spacing: 5

    Item {
      id: loginDetails
      visible: !connected && !debugEngine.waitingForConnection

      width: parent.width
      height: childrenRect.height

      LoginSection {
        id: login

        host: debugEngine.host
        port: debugEngine.port

        onHostChanged: {
          if(debugEngine.host !== host)
          {
            debugEngine.host = host;
          }
        }

        onPortChanged: {
          var intPort = parseInt(port)
          if(debugEngine.port !== intPort)
          {
            debugEngine.port = intPort;
          }
        }

        width: parent.width
      }
    }

    Item {
      id: waitingForConnection
      visible: !connected && debugEngine.waitingForConnection

      width: parent.width
      height: childrenRect.height

      Column {
        width: parent.width
        spacing: 3

        Text {
          width: parent.width
          horizontalAlignment: Text.AlignHCenter
          text: "Listening for Connection at " + debugEngine.host + ":" + debugEngine.port.toString()
        }

        ProgressBar {
          width: parent.width
          height: 20
          indeterminate: true
        }
      }
    }

    Button {
      anchors.horizontalCenter: parent.horizontalCenter
      text: "Connect"
      onClicked: {
        if(debugEngine.waitingForConnection)
        {
          debugEngine.stopListening();
          text = "Connect";
        }
        else if(mainWindow.connected)
        {
          debugEngine.disconnect();
          text = "Connect"
        }
        else
        {
          debugEngine.listen();
          text = "Cancel"
        }
      }
    }
  }
}
