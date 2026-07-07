#version 150 core

in vec4 position;
in vec2 textureCoords;

out vec3 Position;
out vec2 TextureCoords;

uniform mat4 matrixModel;
uniform mat4 matrixView;
uniform mat4 matrixProjection;

void main()
{
    vec4 positionView = matrixView * matrixModel * position;
    vec4 positionProjection = matrixProjection * positionView;

    Position = positionView.xyz;
    TextureCoords = textureCoords;
    gl_Position = positionProjection;
}