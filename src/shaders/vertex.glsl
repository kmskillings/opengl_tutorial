#version 150 core

in vec4 position;
in vec2 texCoordVert;

out vec2 texCoordFrag;

uniform mat4 transform;

void main()
{
    gl_Position = transform * position;
    texCoordFrag = texCoordVert;
}