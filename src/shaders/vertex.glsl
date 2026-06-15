#version 150 core

in vec4 position;
in vec2 textureCoords;

out vec2 TextureCoords;

uniform mat4 transform;

void main()
{
    gl_Position = transform * position;
    TextureCoords = textureCoords;
}