#version 460 core

in vec2 TextureCoords;
in vec3 Color;
in vec3 PositionVertexModel;

out vec4 outColor;

layout(location = 2) uniform sampler2D textureMain;

vec3 spherePosition = vec3(0.5, 0.5, 0.5);
float sphereRadius = 0.5;

void main()
{
    vec3 diff = spherePosition - PositionVertexModel;
    float sdf = dot(diff, diff) - (sphereRadius * sphereRadius);
    if (sdf < 1.0)
    {
        discard;
    }
    
    outColor = texture(textureMain, TextureCoords) * vec4(Color, 1.0);
}