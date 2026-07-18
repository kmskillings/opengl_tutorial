#version 460 core

in vec2 TextureCoords;

out vec4 outColor;

layout(location = 2) uniform sampler2D textureMain;

void main()
{
    outColor = texture(textureMain, TextureCoords);
}