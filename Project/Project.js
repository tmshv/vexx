db.addType({
  name: "Project",
  parent: SEntity,
  properties:
    [
      {
      name: "rootDirectory",
      type: StringProperty
      },
      {
      name: "projectDependancies",
      type: PointerArray
      },
      {
      name: "config",
      type: Pointer
      }
    ],
  prototype:
    {
    filePath: function(config, dir)
      {
      var path = dir + "/BuildProject.proj.json";
      if(config.input)
        {
        path = config.input.rootDirectory.value() + "/" + path;
        }
      return path;
      },
    create: function(config, directory, name)
      {
      var path = this.filePath(config, directory);
      var file = io.createFile(path);
      if(file.open("read"))
        {
        print("Loading from '" + path + "'");
        var pro = db.load("json", file, db.children);

        return pro;
        }

      print("Creating project '" + path + "'");

      var pro = db.addChild(Project, name);
      pro.rootDirectory.setValue(directory);
      pro.config.input = config;
      return pro;
      },
    save: function()
      {
      var path = this.filePath(this.config, this.rootDirectory.value());
      print("Saving to '" + path + "'");

      var file = io.createFile(path);
      file.open("write");
      db.save("json", file, this);
      file.close();
      },
    addDependency: function(dep)
      {
      var newPtr = this.projectDependancies.add();
      newPtr.input = dep;
      return newPtr;
      }
    }
  });
