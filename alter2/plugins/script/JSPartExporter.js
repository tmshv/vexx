var jsFileExporter = {
    name: "Javascript Entity Export",
    fileType: "Javascript Source (*.js)",
    exportFile: function(filename, entity)
        {
        var file = io.createFile(filename);
        if(!file.open("write"))
            {
            print("Couldn't open file " + filename + " for writing.");
            return false;
            }

        var part = entity.children[0];

        var propertyArray = [];
        var subProps = part.children;
        for(var i=0, s=subProps.length; i<s; ++i)
          {
          var subProp = subProps[i];
          print("#" + subProp);
          print("#" + subProp.name);
          /*var obj =
            {
            name: subProp.name,
            type: subProp.typeInformation.typeName
            };*/
          propertyArray.push(obj);
          }

         /*
            [
                {
                    name: "AttrName",
                    type: FloatProperty,
                    affects: [ "Output" ]
                },
                {
                    name: "AttrName2",
                    type: IntProperty,
                    affects: [ "Output" ]
                },
                {
                    name: "Output",
                    type: FloatProperty,
                    compute: function()
                    {
                        this.Output = this.AttrName * this.AttrName2;
                    }
                }
            ]*/

        var nodeObject =
        {
            name: part.name,
            parent: part.typeInformation.typeName,
            properties: propertyArray
        }

        var fileContents = JSON.stringify(nodeObject, null, "  ");

        file.write(fileContents);
        file.close();

        return true;
        }
    };

SPartDocument.registerExporter(jsFileExporter);
