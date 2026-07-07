#version 460 core

in vec2 position;
in vec2 textureCoords;

out vec2 TextureCoords;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
    TextureCoords = textureCoords;
}