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
script.include("Config.js");

// project init.
var config = Config.create("Conf");
  {
  config.rootDirectory.setValue("F:/CodeVexxLocal/");
  }

var EksCore = Project.create(config, "EksCore", "EksCore");

var EksGui = Project.create(config, "EksGui", "EksGui");
EksGui.projectDependancies.addDependancy(EksCore)

ekscore.save();
