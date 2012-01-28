import QtQuick 1.1
import VexxQMLExtensions 1.0

Column {
  id: propertyList
  property alias rootItem: display.rootItem

  function intersect(x, y)
    {
    for(var i = 0, s = display.count; i < s; ++i)
      {
      var item = display.propertyAt(i);
      if(item.intersect)
        {
        var hit = item.intersect(x, y);
        if(hit)
          {
          return hit;
          }
        }
      }
    }

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
