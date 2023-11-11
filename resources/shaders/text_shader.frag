#version 330 core

in vec2 coord;

uniform vec4 color;
uniform sampler2D tex;

out vec4 outColor;

void main()
{
    outColor = color * vec4(1.0, 1.0, 1.0, texture(tex, coord).r);
}