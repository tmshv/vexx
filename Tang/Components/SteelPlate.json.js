db.types.registerType({
  "name": "SteelPlate",
  "parent": "Component",
  "properties": [
    {
      "name": "renderGroup",
      "extend": true,
      "properties": [
        {
          "name": "1",
          "type": "GCRenderablePointer",
          "affects": [],
          "defaultInput": "../renderGroup/1"
        }
      ]
    },
    {
      "name": "Thickness",
      "type": "FloatProperty",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "Width",
      "type": "FloatProperty",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "Height",
      "type": "FloatProperty",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "undefined",
      "type": "MCCuboid",
      "affects": [],
      "properties": [
        {
          "name": "width",
          "extend": true,
          "defaultInput": "../undefined/width"
        },
        {
          "name": "height",
          "extend": true,
          "defaultInput": "../undefined/height"
        },
        {
          "name": "depth",
          "extend": true,
          "defaultInput": "../undefined/depth"
        }
      ]
    }
  ]
});