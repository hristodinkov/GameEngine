#version 400 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, TexCoords);

    float avg = (color.x+color.y+color.z)/3.0f;
    FragColor = vec4(avg,avg,avg,1.0f);
}