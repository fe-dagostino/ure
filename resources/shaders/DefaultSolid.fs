#version 100

precision mediump float;

uniform   vec4      u_v4Color;

void main()
{
  gl_FragColor = u_v4Color;
}
