#version 150 core

in vec3 colorFrag;

out vec4 outColor;

void main()
{
    outColor = vec4(colorFrag, 1.0);
}