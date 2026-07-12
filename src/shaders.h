#ifndef _SHADERS_H
#define _SHADERS_H

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

extern const char* shaderVertexSource;
extern const char* shaderFragmentSource;
extern const char* vertexPhongFacetedSource;
extern const char* fragmentPhongFacetedSource;
extern const char* utilsPhongSource;

GLuint compileShader(
    const char** vertexShaderSources,
    GLuint vertexShaderCount,
    const char** fragmentShaderSources,
    GLuint fragmentShaderCount
);

void reportCompileStatus(GLuint shader);
void reportLinkStatus(GLuint shaderProgram);

#endif