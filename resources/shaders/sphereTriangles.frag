#version 460 core

in vec2 TextureCoords;

layout(location = 1) uniform sampler2D texture2d;

out vec4 outColor;

void main()
{
    outColor = texture(texture2d, TextureCoords);
}