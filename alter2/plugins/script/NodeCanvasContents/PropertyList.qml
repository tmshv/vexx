import QtQuick 1.1
import VexxQMLExtensions 1.0

Column {
  id: propertyList
  property alias rootItem: display.rootItem

  property real xOffset: 0
  property string propertyMask: "inputoutput"
  property bool showInternalProperties: false

  PropertyDisplay {
    id: display

    property: Property {
      xOffset: propertyList.xOffset
    }
  }
}
