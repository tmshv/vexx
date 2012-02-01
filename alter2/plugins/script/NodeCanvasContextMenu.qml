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

  function move(pos)
    {
    menu.x -= pos.x;
    menu.y -= pos.y;

    var newPos = external.position();
    newPos.x += pos.x;
    newPos.y += pos.y;

    external.setPosition(newPos.x, newPos.y);
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
