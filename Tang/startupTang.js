script.includeFolder("Components")

db.types.Component.prototype.addInstance = function(shaderGroup)
  {
  shaderGroup.renderGroup.addPointer(this);
  }

// create a new component document and pop it up.
var area = db.addDocument(db.types.AreaDocument);
area.newFile();

// create a new component document and pop it up.
var component = db.addDocument(db.types.ComponentDocument);
component.newFile();


component.loadFile("F:\\CodeVexxLocal\\Tang\\Components\\SteelPlate.json");
var shaderGroup = db.children[1].children.Scene.children.Groups;
component.children[0].addInstance(shaderGroup);

component.editor = component.createEditor();
component.editor.show();
