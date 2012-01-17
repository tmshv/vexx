import QtQuick 1.1

Column {
  id: propertyList
  property real xOffset: 0
  property alias rootIndex: chilrenVisualModel.rootIndex
  property string propertyMask: "inputoutput"
  property bool showInternalProperties: false

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
      mode: {
        if(propertyMode === "internalinput" && !showInternalProperties)
        {
          return "internal";
        }

        if(propertyMask === "input" && (propertyMode === "output" || propertyMode === "computed"))
        {
          return "internal";
        }

        if(propertyMask === "output" && propertyMode === "input")
        {
          return "internal";
        }

        return propertyMode
      }
      contentsOffset: xOffset
      width: propertyList.width
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
  }
}
