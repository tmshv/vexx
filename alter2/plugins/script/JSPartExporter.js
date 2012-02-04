var jsFileExporter = {
    name: "Javascript Entity Export",
    fileType: "Javascript Source (*.js)",
    exportFile: function(filename, entity)
        {
        db.cancelBlock(function()
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

              if(!subProp.dynamic)
                {
                if(hasExportableChild(subProp))
                  {
                  return true;
                  }
                }
              else
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

              if(!subProp.dynamic)
                {
                if(hasExportableChild(subProp))
                  {
                  var obj =
                    {
                    name: subProp.name,
                    extend: true,
                    properties: []
                    };

                  exportChildren(obj.properties, subProp, true);
                  arr.push(obj);
                  }
                }
              else
                {
                var obj =
                  {
                  name: subProp.name,
                  type: subProp.typeInformation.typeName
                  };

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

                if(doAffects)
                  {
                  obj["affects"] = subProp.affects;
                  }

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

          file.write("db.addType(");
          file.write(fileContents);
          file.write(");");
          file.close();
          });

        return true;
        }
    };

db.types.SPartDocument.registerExporter(jsFileExporter);
