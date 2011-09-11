var debug = true;
if(debug)
  {
  var oldPrint = print;
  script.loadPlugin("ui");
  ui.show();
  script.enableDebugging(true);
  print = oldPrint;
  }

script.loadPlugin("db");

script.include("Project.js");

var ekscore = Project.create("EksCore");
ekscore.save();

print("-- Started Up --");
