import QtQuick 1.1

Column {
    id: propertyList
    property alias rootIndex: chilrenVisualModel.rootIndex

    function childIndex(id)
      {
      return chilrenVisualModel.modelIndex(id);
      }

    function getChildItem(index)
      {
      return properties.itemAt(index);
      }

    VisualDataModel {
        id: chilrenVisualModel
        delegate: Property {
            text: name
            colour: propertyColour
        }
        model: db
    }

    Repeater {
        id: properties
        model: chilrenVisualModel
    }
}
