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
    
    // Calculate where the fragment ray intersects the cutout sphere.

    float a = dot(FragmentPositionView, FragmentPositionView);
    float b = -2.0 * dot(FragmentPositionView, SpherePositionView);
    float c = dot(SpherePositionView, SpherePositionView) - (sphereRadius * sphereRadius);

    float disc = b * b - 4.0 * a * c;
    if (disc <= 0.0)
    {
        discard;
    }
    else
    {
        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
}