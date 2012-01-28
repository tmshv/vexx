var surface = script.addQMLSurface("DebugQMLCanvas", "Dock", "../alter2/plugins/script/NodeCanvas.qml",
    {
    nodePath: "../alter2/plugins/script/NodeCanvasContents/Node.qml"
    }
  );

surface.send.connect(function(name, args)
    {
    print(name, args);
    }
  )
