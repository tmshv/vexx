var debug = true;
if(debug)
  {
  script.loadPlugin("ui");
  ui.show();
  script.enableDebugging(true);
  }

script.loadPlugin("db");

script.include("Project.js");

var ekscore = Project.create("EksCore");

ekscore.save();
