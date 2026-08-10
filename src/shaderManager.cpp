#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderManager.hpp"

#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

extern "C" {
#include "whereami.h"
}

void reportCompileStatus(GLuint shader);
void reportLinkStatus(GLuint shaderProgram);

ShaderManager::ShaderManager(void)
{}

void ShaderManager::init(
    char const * const shadersPath
)
{
    uint pathLength = wai_getExecutablePath(NULL, 0, NULL);
    char * const path = (char*)malloc(pathLength + 1);
    wai_getExecutablePath(path, pathLength, NULL);
    path[pathLength] = '\0';
    std::filesystem::path pathExe(path);
    std::filesystem::path pathContaining = pathExe.parent_path();
    shadersPath_ = pathContaining / shadersPath;
    free(path);
}

uint32_t ShaderManager::loadGlslSource(
    char const * const name
)
{
    uint32_t id = sources_.size() + 1;
    
    std::filesystem::path sourcePath = shadersPath_ / name;
    if (!std::filesystem::is_regular_file(sourcePath))
    {
        return 0;
    }

    // Determine size of file
    uint32_t size = std::filesystem::file_size(sourcePath);

    // Allocate buffer for file
    char * const buf = (char *)malloc(size * sizeof(char) + 1);
    if (buf == NULL)
    {
        return 0;
    }
    
    // Read file into buffer
    std::ifstream stream(sourcePath, std::ios::in | std::ios::binary);
    if (!stream.is_open())
    {
        free(buf);
        return 0;
    }
    stream.read(buf, size);
    buf[size] = '\0';

    sources_.push_back(buf);
    return id;
}

char const * const ShaderManager::getGlslSource(
    uint32_t id
) const
{
    uint32_t index = id - 1;
    if (index < sources_.size())
    {
        return sources_[index];
    }
    else
    {
        return NULL;
    }
}

GLuint ShaderManager::compileShader(
    uint32_t const * const vertexSourceIds,
    uint32_t vertexSourcesCount,
    uint32_t const * const fragmentSourceIds,
    uint32_t fragmentSourcesCount
) const
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLchar const * * vertexSources 
        = (GLchar const * *)malloc(
            sizeof(GLchar const *) * vertexSourcesCount
        );
    for (int i = 0; i < vertexSourcesCount; i++)
    {
        vertexSources[i] = sources_[i];
    }
    glShaderSource(vertexShader, vertexSourcesCount, vertexSources, NULL);
    glCompileShader(vertexShader);
    reportCompileStatus(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar const * * fragmentSources 
        = (GLchar const * *)malloc(
            sizeof(GLchar const *) * fragmentSourcesCount
        );
    for (int i = 0; i < fragmentSourcesCount; i++)
    {
        fragmentSources[i] = sources_[i];
    }
    glShaderSource(fragmentShader, fragmentSourcesCount, fragmentSources, NULL);
    glCompileShader(fragmentShader);
    reportCompileStatus(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    reportLinkStatus(shaderProgram);

    free(vertexSources);
    free(fragmentSources);

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