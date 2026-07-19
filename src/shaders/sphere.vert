#version 460 core

layout(location = 0) in vec3 position;

layout(location = 0) uniform mat4 matrixProjView;

void main()
{
    gl_Position = matrixProjView * vec4(position, 1);
}