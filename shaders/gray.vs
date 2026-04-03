#version 400 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}
