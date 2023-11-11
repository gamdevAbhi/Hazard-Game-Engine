#version 330 core

in vec2 coord;

uniform vec4 color;
uniform sampler2D tex;

out vec4 fragColor;

void main()
{
    fragColor = texture(tex, coord) * color;
}