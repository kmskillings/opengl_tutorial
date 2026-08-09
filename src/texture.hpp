#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

struct Texture
{

    static std::filesystem::path texturesPath;

    GLuint id;
    int width;
    int height;

    Texture(void) :
        id(0),
        width(0),
        height(0)
    {}

    static void init(void);

    static Texture load(
        char const* name
    );

};

#endif