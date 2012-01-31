import QtQuick 1.1
import "CreateMenuContents"

Rectangle {
  id: holder
  width: childrenRect.width+80
  height: childrenRect.height+2
  color: "white"

  function focusLost()
    {
    external.destroyWindow();
    }

  Timer {
    id: grabber
    interval: 1
    onTriggered: {
      external.focus();
      menu.state = "Shown";
    }
  }

  Menu {
    id: menu
    title: "Create..."
  }
}
