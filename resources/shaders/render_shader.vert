#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 coord;

uniform mat4 viewProjMat;
uniform mat4 transformMat;

void main()
{
    gl_Position = viewProjMat * transformMat * vec4(aPos, 1.0f);
    coord = aUV;
}