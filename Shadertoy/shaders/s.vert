#version 110

attribute vec3 position;
uniform float global_time;
uniform vec2  resolution;

varying float iGlobalTime;
varying vec2 iResolution;

void main()
{
    iGlobalTime = global_time;
    iResolution = resolution;
    gl_Position =  vec4(position, 1.0);
}