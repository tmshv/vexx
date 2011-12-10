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

        function exportChildren(arr, parent, onlyDynamic, doAffects)
            {
            for(var i=0, s=parent.length; i<s; ++i)
                {
                var subProp = parent[i];
                if(onlyDynamic && !subProp.dynamic)
                    {
                    continue;
                    }

                var obj =
                    {
                    name: subProp.name,
                    type: subProp.typeInformation.typeName
                    };

                var val = subProp.valueString;
                if(val != "")
                    {
                    obj["defaultValue"] = val;
                    }

                if(doAffects)
                    {
                    obj["affects"] = subProp.affects;
                    }

                arr.push(obj);
                }
            }

        var propertyArray = [];
        var subProps = part.children;
        exportChildren(propertyArray, subProps, true, false);
        exportChildren(propertyArray, part, true, true);

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
