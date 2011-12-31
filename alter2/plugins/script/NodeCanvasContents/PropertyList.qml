import QtQuick 1.1

Column {
    id: propertyList
    property alias rootIndex: chilrenVisualModel.rootIndex

    function childIndex(id)
    {
      return chilrenVisualModel.modelIndex(id);
    }

    VisualDataModel {
        id: chilrenVisualModel
        delegate: Property {
            text: name
            colour: propertyColour
            input: propertyInput
        }
        model: db
    }

    Repeater {
        id: properties
        model: chilrenVisualModel
    }
}
