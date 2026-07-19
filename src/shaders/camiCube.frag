#version 460 core

in vec3 Position;
in vec2 TextureCoords;

out vec4 outColor;

layout(location = 2) uniform sampler2D textureMain;
layout(location = 3) uniform vec3 spherePosition;
layout(location = 4) uniform float sphereRadius;

void main()
{
    vec3 distanceFromSphere = Position - spherePosition;
    if (
        dot(distanceFromSphere, distanceFromSphere) < 
        dot(sphereRadius, sphereRadius)
    )
    {
        discard;
    } else {
        outColor = texture(textureMain, TextureCoords);
    }
}