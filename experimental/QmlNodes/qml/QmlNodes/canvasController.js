var component;
var sprite;


function createNodeObjects()
  {
  foreach(n in viewData)
    {
    var node = viewData[n];
    var data =
      {
      "x": node.x,
      "y": node.y,
      name: n,
      path: path,
      type: node.type
      };
    
    sprite = n.qmlType.createObject(container, n);
    
    if (sprite == null)
      {
      // Error Handling
      console.log("Error creating object");
      }
    }
  }