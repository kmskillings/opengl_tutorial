#version 460 core

layout(location = 1) uniform vec3 color;

out vec4 outColor;

void main()
{
    outColor = vec4(color, 1.0);
}