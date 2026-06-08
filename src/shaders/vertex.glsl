#version 150 core

in vec2 position;
in vec2 texCoordVert;

out vec2 texCoordFrag;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texCoordFrag = texCoordVert;
}