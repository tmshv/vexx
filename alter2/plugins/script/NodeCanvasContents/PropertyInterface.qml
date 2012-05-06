// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import VexxQMLExtensions 1.0

Ellipse {
  id: ellipse
  property alias size: ellipse.width

  function attachPoint(rel)
    {
    var pt = ellipse.mapToItem(rel, size/2, size/2+2);
    return Qt.point(pt.x, pt.y);
    }

  border.color: Qt.darker(color, 1.4)
  border.width: 1
  height: width
  }
