attribute vec3 vertex;
void main(void)
  {
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(vertex,1.0);
  }
