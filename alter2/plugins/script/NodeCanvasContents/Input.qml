import QtQuick 1.1
import VexxQMLExtensions 1.0

ConnectorItem {
  id: inputHolder

  Path {
    width: 4

    firstColour: driverColour
    lastColour: drivenColour

    firstNormal.x: driverNormal.x * 100
    firstNormal.y: driverNormal.y * 100
    lastNormal.x: drivenNormal.x * 100
    lastNormal.y: drivenNormal.y * 100

    firstPoint: {
      return inputHolder.driverPoint
    }
    lastPoint: {
      return inputHolder.drivenPoint
    }
  }
}
