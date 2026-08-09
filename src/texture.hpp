#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Texture
{

    GLuint id;
    int width;
    int height;

    Texture(void) :
        id(0),
        width(0),
        height(0)
    {}

    static Texture load(
        char const* path
    );

};

#endif