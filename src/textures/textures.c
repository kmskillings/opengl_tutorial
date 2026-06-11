#include <stdlib.h>
#include <stdbool.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include <glfw3.h>

#include "textures.h"
#include "texture_bytes.h"

void initializeTextures(void)
{
    textureCami = createTexture(GL_TEXTURE0, 1024, 1024, textureBytesCami);
}

Texture* createTexture(
    GLenum imageUnit,
    GLuint width,
    GLuint height,
    char* bytes
)
{
    Texture* texture = malloc(sizeof(Texture));
    if (texture == NULL) {
        goto fail_texture;
    }

    texture->imageUnit = imageUnit;
    glGenTextures(1, &(texture->textureName));
    glActiveTexture(imageUnit);
    glBindTexture(GL_TEXTURE_2D, texture->textureName);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);

    return texture;

fail_texture:
    return NULL;
    
}