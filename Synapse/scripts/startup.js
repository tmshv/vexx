assert(script.loadPlugin("synapsecore"));

script.includeFolder("./nodes");

ui.show();

var mainDocument = db.addChild(db.types.SyDocument, "MainDocument");

//db.addChild("SyImageNode", "Image");
//db.addChild("SyImageNode", "Image2");
//db.addChild("Add", "Add");

//db.children.Image.filename.setValue("C:\\Users\\Jorj\\Pictures\\02432_elephantastic_1280x800.jpg");
//db.children.Image2.filename.setValue("C:\\Users\\Jorj\\Pictures\\02432_elephantastic_1280x800.jpg");

//db.children.Add.InputA.input = db.children.Image.output;
//db.children.Add.InputB.input = db.children.Image2.output;

//print(db.children.Add.Output.image.at(0, 0));
