void main(void)
  {
  float dist = gl_FragCoord.x + gl_FragCoord.y;
  bool aOrB = fmod(dist, 20.0) < 10.0;
  if(aOrB)
    {
    gl_FragColor = vec4( 0.2, 0.2, 0.2, 1.0 );
    }
  else
    {
    gl_FragColor = vec4( 1.0, 1.0, 0.0, 1.0 );
    }
  }
