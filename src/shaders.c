#include "shaders.h"
#include <stdio.h>

void reportCompileStatus(GLuint shader);
void reportLinkStatus(GLuint shaderProgram);

const char vertexShaderSourceArray[] = {
    #include "shaders/textured.vert.xxd"
    , 0x00
};
const char* shaderVertexSource = vertexShaderSourceArray;

const char fragmentShaderSourceArray[] = {
    #include "shaders/textured.frag.xxd"
    , 0x00
};
const char* shaderFragmentSource = fragmentShaderSourceArray;

unsigned int compileShader(
    const char** vertexShaderSources,
    GLuint vertexShaderCount,
    const char** fragmentShaderSources,
    GLuint fragmentShaderCount
)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, vertexShaderCount, vertexShaderSources, NULL);
    glCompileShader(vertexShader);
    reportCompileStatus(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, fragmentShaderCount, fragmentShaderSources, NULL);
    glCompileShader(fragmentShader);
    reportCompileStatus(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    reportLinkStatus(shaderProgram);

    return shaderProgram;
}

void reportCompileStatus(GLuint shader)
{
    GLint shaderStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus == GL_TRUE) return;

    char log[512];
    glGetShaderInfoLog(shader, 511, NULL, log);
    printf("%s", log);

}

void reportLinkStatus(GLuint shaderProgram)
{
    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_TRUE) return;

    char log[512];
    glGetProgramInfoLog(shaderProgram, 511, NULL, log);
    printf("%s", log);
}