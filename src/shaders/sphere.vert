#version 460 core

layout(location = 0) in vec3 position;

layout(location = 0) uniform mat4 matrixProjViewModel;

void main()
{
    gl_Position = matrixProjViewModel * vec4(position, 1);
}