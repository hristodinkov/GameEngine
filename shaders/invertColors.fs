#version 400 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, TexCoords);

    vec3 inverted = vec3(1.0) - color.rgb;
    FragColor = vec4(inverted, 1.0);
}
