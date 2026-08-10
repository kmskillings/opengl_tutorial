#ifndef _SHADER_MANAGER_HPP
#define _SHADER_MANAGER_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <vector>

struct GlslSource;

class ShaderManager
{

private:

    std::filesystem::path shadersPath_;

    std::vector<char const *> sources_;

public:

    ShaderManager(void);

    void init(
        char const * const shadersPath
    );

    uint32_t loadGlslSource(
        char const * const name
    );

    char const * const getGlslSource(
        uint32_t id
    ) const;

    GLuint compileShader(
        uint32_t const * const vertexSourceIds,
        uint32_t vertexSourcesCount,
        uint32_t const * const fragmentSourceIds,
        uint32_t fragmentSourcesCount
    ) const;

};

#endif