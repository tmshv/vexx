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
    filePath: function(config, dir, name)
      {
      var path = dir + "/" + name + ".proj.json";
      if(config.input)
        {
        path = config.input.rootDirectory.value() + "/" + path;
        }
      return path;
      },
    create: function(config, directory, name)
      {
      var path = this.filePath(config, directory, name);
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
      var path = this.filePath(this.config, this.rootDirectory.value(), this.name);
      print("Saving to '" + path + "'");

      var file = io.createFile(path);
      file.open("write");
      db.save("json", file, this);
      file.close();
      }
    }
  });
