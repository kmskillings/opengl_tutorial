#version 460 core

in vec2 TextureCoords;

out vec4 outColor;

uniform sampler2D textureScreen;

void main()
{
    outColor = texture(textureScreen, TextureCoords);
}