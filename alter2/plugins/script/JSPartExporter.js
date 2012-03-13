var jsFileExporter = {
    name: "Javascript Entity Export",
    fileType: "Javascript Source (*.js)",
    exportFile: function(filename, entity)
      {
      entity.cancelBlock(function()
        {
        var file = io.createFile(filename);
        if(!file.open("write"))
          {
          print("Couldn't open file " + filename + " for writing.");
          return false;
          }

        var part = entity.children[0];

        function hasExportableChild(parent)
          {
          for(var i=0, s=parent.length; i<s; ++i)
            {
            var subProp = parent[i];

            if(!subProp.dynamic && subProp.input === null)
              {
              if(hasExportableChild(subProp))
                {
                return true;
                }
              }
            else if(subProp.mode !== "internal" && subProp.mode !== "internalcompute")
              {
              return true;
              }
            }
          return false;
          }

        function exportChildren(arr, parent, doAffects, excludeArray)
          {
          for(var i=0, s=parent.length; i<s; ++i)
            {
            var subProp = parent[i];

            if(excludeArray && excludeArray.indexOf(subProp.name) !== -1)
              {
              continue;
              }

            var mode = subProp.mode;
            if(mode === "internal" || mode === "internalcomputed")
              {
              continue;
              }

            var doExport = false;
            var obj =
              {
              name: subProp.name,
              };
            var dynamic = subProp.dynamic;
            if(!dynamic)
              {
              obj["extend"] = true;
              }

            print(subProp.name, " ", mode);
            if(dynamic || subProp.input !== null)
              {
              doExport = true;
              if(dynamic)
                {
                obj["type"] = subProp.typeInformation.typeName;

                if(doAffects)
                  {
                  obj["affects"] = subProp.affects;
                  }
                }

              var input = subProp.input;
              if(input)
                {
                obj["defaultInput"] = subProp.pathTo(input);
                }
              else
                {
                var val = subProp.valueString;
                if(val !== "")
                  {
                  obj["defaultValue"] = val;
                  }
                }
              }

            if(hasExportableChild(subProp))
              {
              doExport = true;

              obj["properties"] = [];
              exportChildren(obj.properties, subProp, true);
              }

            if(doExport)
              {
              arr.push(obj);
              }
            }
          }


        var nodeObject =
          {
          name: part.name,
          parent: part.typeInformation.typeName,
          properties: []
          }

        var children = part.children;
        for(var i=0, s=children.length; i<s; ++i)
          {
          var subProp = children[0];

          subProp.parent = part;
          }

        exportChildren(nodeObject.properties, part, true, [ "children" ]);

        var fileContents = JSON.stringify(nodeObject, null, "  ");

          file.write("db.types.addType(");
          file.write(fileContents);
          file.write(");");
          file.close();
          });

      return true;
      }
    };

db.types.registerExporter(db.types.SPartDocument, jsFileExporter);
