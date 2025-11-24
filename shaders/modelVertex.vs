#version 400 core

layout (location = 0) in vec3 aPos; //object-space vertex position
layout (location = 1) in vec3 aNor; //object-space normal vector
layout (location = 2) in vec2 aUv; //texture coordinates

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec3 worldPos;
out vec3 worldNormal;
out vec2 uv;

void main() {
     vec4 wPos = modelMatrix* vec4(aPos,1.0);
     worldPos = wPos.xyz;

     worldNormal = mat3(transpose(inverse(modelMatrix)))*aNor;
     uv=aUv;

     gl_Position = projMatrix * viewMatrix * wPos;
}