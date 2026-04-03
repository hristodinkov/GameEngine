#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * viewMatrix * vec4(aPos, 1.0);
}
