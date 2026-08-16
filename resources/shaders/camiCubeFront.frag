#version 460 core

in vec2 TextureCoords;
in vec3 Color;
in vec3 FragmentPositionView;
in vec3 SpherePositionView;

out vec4 outColor;

layout(location = 3) uniform sampler2D textureMain;

float sphereRadius = 0.5;

void main()
{
    vec3 diff = SpherePositionView - FragmentPositionView;
    float sdf = dot(diff, diff) - (sphereRadius * sphereRadius);
    if (sdf < 0.0)
    {
        discard;
    }
    
    outColor = texture(textureMain, TextureCoords) * vec4(Color, 1.0);
}