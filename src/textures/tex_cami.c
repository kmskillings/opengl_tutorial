#include <stdlib.h>
#include <stdbool.h>

#define GLEW_NO_GLU
#define GLEW_STATIC
#include <glew.h>

#include <glfw3.h>

#include "textures.h"

char textureBytesCami[] = {
    #include "cami.xxd"
};