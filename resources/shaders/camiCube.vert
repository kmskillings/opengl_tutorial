#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in float modelScale;
layout(location = 3) in float modelOrientationAngle;
layout(location = 4) in vec3 modelOrientationAxis;
layout(location = 5) in float modelRotationRate;
layout(location = 6) in vec3 modelRotationAxis;
layout(location = 7) in vec3 modelPosition;
layout(location = 8) in vec3 color;

out vec2 TextureCoords;
out vec3 FragmentPositionView;
out vec3 SpherePositionView;
out vec3 Color;

layout(location = 0) uniform mat4 matrixProj;
layout(location = 1) uniform mat4 matrixView;
layout(location = 2) uniform float secondsElapsed;

vec3 rotateAngleAxis(vec3 v, float th, vec3 k)
{
    float c = cos(th);
    float s = sin(th);
    return
        v * c                   + 
        cross(k, v) * s         +
        k * dot(k, v) * (1 - c)
    ;
}

vec3 modelToWorld(vec3 v)
{
    vec3 result = v * modelScale;
    result = rotateAngleAxis(
        result, 
        modelOrientationAngle, 
        modelOrientationAxis
    );
    result = rotateAngleAxis(
        result,
        modelRotationRate * secondsElapsed,
        modelRotationAxis
    );
    result = result + modelPosition;
    return result;
}

void main()
{
    vec3 worldPos = modelToWorld(position);
    vec3 spherePositionWorld = modelToWorld(vec3(0.5, 0.5, 0.5));

    vec3 viewPos = vec3(matrixView * vec4(worldPos, 1.0));
    FragmentPositionView = viewPos;
    gl_Position = matrixProj * vec4(viewPos, 1.0);
    TextureCoords = textureCoords;
    SpherePositionView = vec3(matrixView * vec4(spherePositionWorld, 1.0));
    Color = color;
}
