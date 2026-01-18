#version 400
in vec2 TexCoords;
uniform sampler2D screenTexture;
out vec4 frag_colour;


void main()
{
    float offset = 0.001;

    vec2 offsets[9] = vec2[ ](
    vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
    vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2( offset, 0.0f),
    vec2(-offset, -offset), vec2(0.0f, -offset), vec2( offset, -offset));

    float kernel[9] = float[ ](
    1.0, 1.0, 1.0,
    1.0, -8.0, 1.0,
    1.0, 1.0, 1.0
    );

    vec4 col = vec4(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += texture(screenTexture, TexCoords.st + offsets[i]) * kernel[i];
    }
    col.a = 1.0;
    frag_colour = col;
}