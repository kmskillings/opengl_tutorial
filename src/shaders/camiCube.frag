#version 460 core

in vec2 TextureCoords;
in vec3 Color;

out vec4 outColor;

layout(location = 2) uniform sampler2D textureMain;

void main()
{
    outColor = texture(textureMain, TextureCoords) * vec4(Color, 1.0);
}