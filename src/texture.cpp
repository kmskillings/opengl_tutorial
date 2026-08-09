#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>

extern "C" {
#include "whereami.h"
}

std::filesystem::path Texture::texturesPath;

void Texture::init(void)
{
    uint pathLength = wai_getExecutablePath(NULL, 0, NULL);
    char* path = (char*)malloc(pathLength + 1);
    wai_getExecutablePath(path, pathLength, NULL);
    path[pathLength] = '\0';

    std::filesystem::path pathExe(path);
    std::filesystem::path pathContaining = pathExe.parent_path();
    Texture::texturesPath = pathContaining / "resources" / "textures";

}

Texture Texture::load(
    char const* name
)
{
    std::filesystem::path path = Texture::texturesPath / name;
    Texture texture;
    uint8_t* data = stbi_load(
        path.c_str(),
        &texture.width,
        &texture.height,
        NULL,
        4
    );

    if (data == NULL)
    {
        return texture;
    }

    glGenTextures(1, &texture.id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        texture.width,
        texture.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
}