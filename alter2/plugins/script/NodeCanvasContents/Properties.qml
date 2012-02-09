import QtQuick 1.1

Rectangle {
  id: properties
  width: 250
  height: 50
  smooth: true
  border.color: "#666666"
  radius: 4
  color: "#282828"

  Component.onCompleted: {
    var actualSize = nodeItem.createPropertiesSection(properties, width, height);

    width = actualSize.width;
    height = actualSize.height;
    }
}
