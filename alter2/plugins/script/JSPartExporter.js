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

        var part = entity.children.firstChild();

        var propertyArray = []; /*
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
