import QtQuick 1.1

Rectangle {
  id: inputHolder
  property variant inputIndex: null

  width: 3
  height: 3
  color: "red"

  Text {
    id: text
  }

  function update()
    {
    var items = nodecanvas.findPropertyItem(input);
    if(items)
      {
      var lastItem = items[items.length-1];
      var mapped = mapFromItem(lastItem, 0, 0);

      inputHolder.x = mapped.x;
      inputHolder.y = mapped.y;
      }
    }

  function setupInput()
    {
    var items = nodecanvas.findPropertyItem(input);
    if(items)
      {
      for(var i = 0, s = items.length; i < s; ++i)
        {
        var item = items[i];
        item.xChanged.connect(inputHolder.update);
        item.yChanged.connect(inputHolder.update);
        }
      }

    update();
    }

  Component.onCompleted: setupInput();
}
