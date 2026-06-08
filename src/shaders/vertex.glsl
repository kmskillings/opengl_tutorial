#version 150 core

in vec2 position;
in vec3 colorVert;

out vec3 colorFrag;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    colorFrag = colorVert;
}