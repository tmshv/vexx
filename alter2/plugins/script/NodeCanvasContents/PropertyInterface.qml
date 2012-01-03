// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import VexxQMLExtensions 1.0

Ellipse {
  id: ellipse
  property alias size: ellipse.width
  border.color: Qt.darker(color, 1.4)
  border.width: 1
  height: width
}
