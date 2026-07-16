#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in mat4 matrixModel;

out vec2 TextureCoords;

layout(location = 0) uniform mat4 matrixProjView;

void main()
{
    gl_Position = matrixProjView * matrixModel * vec4(position, 1.0f);
    TextureCoords = textureCoords;
}