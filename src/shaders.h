#ifndef _SHADERS_H
#define _SHADERS_H

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;
extern const char* vertexPhongFacetedSource;
extern const char* fragmentPhongFacetedSource;
extern const char* utilsPhongSource;

GLuint compileShader(
    const char** vertexShaderSources,
    GLuint vertexShaderCount,
    const char** fragmentShaderSources,
    GLuint fragmentShaderCount
);

#endif