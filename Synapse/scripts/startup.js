assert(script.loadPlugin("synapsecore"));

var viewer = synapse.createViewer();
ui.addDock("Preview", viewer);

script.includeFolder("./nodes");

var mainDocument = db.addChild(db.types.SyDocument, "MainDocument");

viewer.setInput(mainDocument.input);

ui.show();

var blacklistedTypes =
  [
  "SySourceNode"
  ];

function addType(name)
  {
  var type = db.types[name];
  if(type.inheritsFrom(db.types.SyNode))
    {
    if(blacklistedTypes.indexOf(type.typeName) === -1)
      {
      userCreatableEntities.push(type);
      }
    }
  }

function removeType(name)
  {
  var type = db.types[name];
  }

script.typeAdded.connect(addType);
script.typeRemoved.connect(addType);

for(var a in db.types)
  {
  addType(a);
  }

//db.addChild("SyImageNode", "Image");
//db.addChild("SyImageNode", "Image2");
//db.addChild("Add", "Add");

//db.children.Image.filename.setValue("C:\\Users\\Jorj\\Pictures\\02432_elephantastic_1280x800.jpg");
//db.children.Image2.filename.setValue("C:\\Users\\Jorj\\Pictures\\02432_elephantastic_1280x800.jpg");

//db.children.Add.InputA.input = db.children.Image.output;
//db.children.Add.InputB.input = db.children.Image2.output;

//print(db.children.Add.Output.image.at(0, 0));
