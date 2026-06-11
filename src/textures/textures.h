#ifndef _TEXTURES_H
#define _TEXTURES_H

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

typedef struct {
    GLenum imageUnit;
    GLuint textureName;
} Texture;

extern Texture* textureCami;

void initializeTextures(void);

Texture* createTexture(
    GLenum imageUnit,
    GLuint width,
    GLuint height,
    char* bytes
);

#endif