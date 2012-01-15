import QtQuick 1.1

Column {
  id: propertyList
  property real xOffset: 0
  property alias rootIndex: chilrenVisualModel.rootIndex

  function childIndex(id)
    {
    return chilrenVisualModel.modelIndex(id);
    }

  function childItem(index)
    {
    return properties.itemAt(index);
    }

  VisualDataModel {
    id: chilrenVisualModel
    model: db

    Property {
      text: name
      colour: propertyColour
      mode: propertyMode
      contentsOffset: xOffset
      width: propertyList.width
    }

    onItemsInserted: {
      print("Properties added", index, count);
    }
  }

  Repeater {
    id: properties
    model: chilrenVisualModel

    /*onItemRemoved: {
      print("Remove input?");
      print(item.text);
      print(item.input);
    }*/

    onItemAdded: {
      print("b", item, item.visible, item.text);
    }
  }
}
