#version 150 core

in vec4 position;
in vec2 texCoordVert;

out vec2 texCoordFrag;

void main()
{
    gl_Position = position;
    texCoordFrag = texCoordVert;
}