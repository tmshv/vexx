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
    var data = nodeItem.createPropertiesSection(properties, "white", width, height);

    if(!data.anyControls)
      {
      width = 0;
      height = 0;
      }
    else
      {
      width = data.width;
      height = data.height;
      }
    }
}
