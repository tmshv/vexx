script.includeFolder("Components")

db.types.Component.prototype.addInstance = function(shaderGroup)
  {
  shaderGroup.renderGroup.addPointer(this);
  }

// create a new component document and pop it up.
var area = db.addDocument(db.types.AreaDocument);
area.newFile();

var viewportWidget = tang.createViewport();
ui.addDock("Viewport", viewportWidget);

var createBasicScene = function(viewportWidget)
  {
  var dbViewport = viewportWidget.viewport();

  var cam = dbViewport.addChild(db.types.GCPerspectiveCamera, "Camera");
  cam.viewportX.input = dbViewport.x;
  cam.viewportY.input = dbViewport.y;
  cam.viewportWidth.input = dbViewport.width;
  cam.viewportHeight.input = dbViewport.height;

  cam.setPosition(0.0, 0.0, 10.0);
  cam.setFocalPoint(0.0, 0.0, 0.0);

  var msc = dbViewport.addChild(db.types.GCManipulatableScene,"Scene");

  msc.setCamera(cam);
  viewportWidget.setScene(msc);

  return msc;
  }

var msc = createBasicScene(viewportWidget);


var shadingGroup = msc.addChild(db.types.GCShadingGroup, "Groups");
msc.renderGroup.addPointer(shadingGroup);

var shader = msc.addChild(db.types.StandardSurface, "Shader");
shadingGroup.shader.input = shader;


// create a new component document and pop it up.
var component = db.addDocument(db.types.ComponentDocument);
component.newFile();


component.loadFile("..\\Tang\\Components\\SteelPlate.json");
component.children[0].addInstance(shadingGroup);

component.editor = component.createEditor();
component.editor.show();

ui.show();
