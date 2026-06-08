#version 150 core

uniform sampler2D textureCami;

in vec2 texCoordFrag;

out vec4 outColor;

void main()
{
    vec4 colorCami = texture(textureCami, texCoordFrag);
    outColor = colorCami;
}