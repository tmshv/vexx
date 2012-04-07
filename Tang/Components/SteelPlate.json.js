db.types.addType({
  "name": "SteelPlate",
  "properties": [
    {
      "name": "RenderGroups",
      "affects": [],
      "properties": [
        {
          "name": "0",
          "affects": [],
          "defaultInput": "../RenderGroups/0"
        }
      ]
    },
    {
      "name": "Thickness",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "Size",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "1",
      "affects": [],
      "properties": [
        {
          "name": "width",
          "extend": true,
          "defaultInput": "../1/width"
        },
        {
          "name": "height",
          "extend": true,
          "defaultInput": "../1/height"
        },
        {
          "name": "depth",
          "extend": true,
          "defaultInput": "../1/depth"
        }
      ]
    },
    {
      "name": "3",
      "affects": [],
      "properties": [
        {
          "name": "geometry",
          "extend": true,
          "defaultInput": "../3/geometry"
        }
      ]
    }
  ]
});