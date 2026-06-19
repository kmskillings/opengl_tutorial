#version 150 core

in vec4 position;
in vec2 textureCoords;

out Position;
out TextureCoords;

uniform matrixModel;
uniform matrixView;
uniform matrixProjection;

void main()
{
    vec4 positionView = matrixView * matrixModel * position;
    vec4 positionProjection = matrixProjection * positionProjection;

    Position = positionView;
    TextureCoords = textureCoords;
    gl_Position = positionProjection;
}