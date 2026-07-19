#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in float modelScale;
layout(location = 3) in float modelOrientationAngle;
layout(location = 4) in vec3 modelOrientationAxis;
layout(location = 5) in float modelRotationRate;
layout(location = 6) in vec3 modelRotationAxis;
layout(location = 7) in vec3 modelPosition;

out vec3 Position;
out vec2 TextureCoords;

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

void main()
{
    float modelRotationAngle = modelRotationRate * secondsElapsed;
    vec3 outPos = position * modelScale;
    outPos = rotateAngleAxis(
        outPos,
        modelOrientationAngle,
        modelOrientationAxis
    );
    outPos = rotateAngleAxis(
        outPos,
        modelRotationAngle,
        modelRotationAxis
    );
    outPos = outPos + modelPosition;

    Position = (matrixView * vec4(outPos, 1.0f)).xyz;
    vec4 outPos4 = matrixProj * matrixView * vec4(outPos, 1.0f);

    gl_Position = outPos4;
    TextureCoords = textureCoords;
}
