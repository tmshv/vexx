SPartDocument.registerExporter
    ({
    name: "Javascript Entity Export",
    fileType: "Javascript Source (*.js)",
    exportFile: function(filename, entity)
        {
                    print("shit going down." + filename + entity.toString());
        }
    });
