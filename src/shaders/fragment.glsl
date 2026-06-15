#version 150 core

in vec2 TextureCoords;

out vec4 outColor;

uniform sampler2D textureMain;

void main()
{
    outColor = texture(textureMain, TextureCoords);
}