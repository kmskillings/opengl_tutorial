#ifndef _SHADERS_H
#define _SHADERS_H

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

extern const char* camiCubeVertexSource;
extern const char* camiCubeFragmentSource;
extern const char* sphereVertexSource;
extern const char* sphereFragmentSource;

GLuint compileShader(
    const char** vertexShaderSources,
    GLuint vertexShaderCount,
    const char** fragmentShaderSources,
    GLuint fragmentShaderCount
);

void reportCompileStatus(GLuint shader);
void reportLinkStatus(GLuint shaderProgram);

#endif