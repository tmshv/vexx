assert(script.loadPlugin("synapsecore"));

script.includeFolder("./nodes");

db.addChild("SyImageNode", "Image");
db.addChild("SyImageNode", "Image2");
db.addChild("Add", "Add");

db.children.Image.filename.setValue("/Users/steviedenyer/Halong.jpg");
db.children.Image2.filename.setValue("/Users/steviedenyer/Halong.jpg");

db.children.Add.InputA.input = db.children.Image.output
db.children.Add.InputB.input = db.children.Image2.output
