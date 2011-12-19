db.addType({
  "name": "StandardSurface",
  "parent": "GCShader",
  "properties": [
    {
      "name": "components",
      "extend": true,
      "properties": [
        {
          "name": "1",
          "type": "GCShaderComponentPointer",
          "defaultInput": "../../Vert"
        },
        {
          "name": "2",
          "type": "GCShaderComponentPointer",
          "defaultInput": "../../Frag"
        }
      ]
    },
    {
      "name": "Vert",
      "type": "GCVertexShaderComponent",
      "defaultValue": "attribute vec3 vertex;\nattribute vec3 normals;\nvarying vec3 normal;\n\nvoid main(void)\n  {\n  normal = normals;\n  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(vertex,1.0);\n  }\n"
    },
    {
      "name": "Frag",
      "type": "GCFragmentShaderComponent",
      "defaultValue": "varying vec3 normal;\n\nvoid main(void)\n  {\n  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n  }\n"
    }
  ]
});