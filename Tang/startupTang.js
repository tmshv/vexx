script.includeFolder("Components")

db.types.Component.prototype.addInstance = function(shaderGroup)
  {
  shaderGroup.renderGroup.addPointer(this);
  }

// create a new component document and pop it up.
var document = db.addDocument(db.types.ComponentDocument);
document.newFile();


document.loadFile("F:\\CodeVexxLocal\\Tang\\Components\\SteelPlate.json");
var shaderGroup = db.children[1].children.Scene.children.Groups;
document.children[0].addInstance(shaderGroup);

document.editor = document.createEditor();
document.editor.show();
