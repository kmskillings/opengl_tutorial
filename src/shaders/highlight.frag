#version 460 core

out vec4 outColor;

layout(location = 0) uniform vec3 color;

void main()
{
    outColor = vec4(color, 1.0);
}